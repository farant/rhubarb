# Visiones extractae — text-stack-visio.md

*Extracted and classified 2026-10-02 from `project-specs/text-stack-visio.md`
(2026-09-21, author given as "@Someone"; it reads as a research brief,
and its addendum summarises Fran's Brighton note). Not Fran's words. One
claim per bullet, worded as close to the source as possible; `Lnn` =
source line. The last section is my commentary, kept apart.*

*Marking: put `[+]` on the claims to keep for the distillate.*

*Tags (see `docs/taxonomiae.html`): `{PRINCIPIUM}` prime condition,
`{PRINCIPIUM+intervention, split}` a condition with its cure attached,
`{OCCASIO}` opportunity, `{INDICIUM}` clue, `{REGULA}` commitment, `{VISIO-OPERIS}` project vision, `{DECRETUM}`
load-bearing decision, `{SUPELLEX}` furniture, `{nulla:…}` fits no tier
(ratio, exemplum, quaestio). `> Xn` = the item(s) it is evidence for; `> Rn` = the project-wide regula it bears on.*

## I. The thesis

- [ ] X1 Build one in-house C89 text stack (font parsing, shaping, rasterization, Unicode, paragraph layout, PDF read/write) rather than vendoring FreeType, HarfBuzz and ICU separately (L7)  {VISIO-OPERIS}
- [ ] X2 The three incumbents are "a single problem split by history", and a from-scratch project is "the one moment the seams can be erased" (L7)  {PRINCIPIUM}
  - [ ] X2.1 FreeType and HarfBuzz each parse the same font tables and glue themselves together through indirection that exists only because they are separate binaries; ICU is "where Unicode tables happened to live" (L9)  {OCCASIO}
  - [ ] X2.2 what the text engine needs from ICU is already inside the shaper's dependencies, so combining them is "not integration work but the absence of it" (L9)  {OCCASIO}
- [ ] X3 PDF is the display list the layout engine already produces: a PDF writer is a serializer and a reader a deserializer of one shared structure (L9, L118)  {OCCASIO}
- [ ] X4 "The oracles already exist": HarfBuzz's shape corpus, Unicode's conformance files and FreeType's output give exact or near-exact targets for every layer except perceptual quality (L9)  {OCCASIO}
- [ ] X5 What it buys (L11)  {VISIO-OPERIS}
  - [ ] X5.1 one sanitized table view, one variation-coordinate set, one glyph cache, one rasterizer for glyphs and page graphics (L11)  {VISIO-OPERIS}
  - [ ] X5.2 byte offsets into UTF-8 as the single currency for cursors, search, annotations and PDF ToUnicode (L11)  {VISIO-OPERIS}
  - [ ] X5.3 a single debugging surface from font bytes to printed page (L11, L134)  {VISIO-OPERIS}
- [ ] X6 What it costs: no swapping a half for its upstream later; roughly 60–100k lines of exacting code; a rendering-quality tier no test suite can judge (L13)  {nulla:ratio > R3}
  - [ ] X6.1 for a 1.0-and-done project the first cost is nil, AI-assisted volume makes the second affordable, and the third "is where taste has to be spent" (L13)  {nulla:ratio > R1}

## II. Why now

- [ ] X7 The unified design is proven in production, in Rust: Google's fontations put outlines on one sanitized parser (`read-fonts`, `skrifa`), and HarfBuzz was ported onto it as HarfRust (L19)  {INDICIUM}
  - [ ] X7.1 Chrome removed FreeType from Blink in release 145; typst is migrating (L19)  {INDICIUM}
  - [ ] X7.2 so the cleanest reference implementations to read are now the Rust ports (L19)  {SUPELLEX}
- [ ] X8 ICU4X is the Unicode Consortium's "sanctioned admission that ICU's monolith was an accident of the 1990s", and a demonstration of the generator-plus-tables architecture (L21)  {INDICIUM}
- [ ] X9 AI-assisted volume changes what one person can carry: the stack's difficulty "is care, not invention" (L23)  {OCCASIO > R3}
  - [ ] X9.1 every layer has a specification and most a conformance corpus: "precisely the shape of work where an AI produces the bulk and the human spends attention on the oracle results" and on judgement (L23)  {OCCASIO}
- [ ] X10 "Nobody vendors these libraries to swap them later; they vendor them because rewriting looked impossible" (L25)  {PRINCIPIUM > R3}
  - [ ] X10.1 when the release is final and the code owned, the option value of a swappable upstream is worth nothing, and the cost of every seam is paid forever (L25)  {nulla:ratio > X10, R1}

## III. The incumbents

- [ ] X11 FreeType is the only C citizen, and the only one whose behaviour is not externally specified: its coverage values are "simply what its algorithm produces" (L37)  {nulla:ratio}
- [ ] X12 HarfBuzz's corpus encodes platform-compatibility decisions, which is valuable because fonts in the wild assume them (L37, L156)  {nulla:ratio}
- [ ] X13 ICU's most valuable asset is not its code but the conformance files, which come from unicode.org whether or not ICU is used (L37)  {OCCASIO}
- [ ] X14 Unicode's stability policies freeze normalization, case mapping and properties, so a table snapshot ages gracefully; only time zones genuinely rot (L39)  {OCCASIO}

## IV. The seams

- [ ] X15 Every seam cost exists only because three libraries are three binaries: "none is a design decision anyone would make starting fresh" (L43)  {nulla:ratio > X2}
  - [ ] X15.1 two font parsers, with two sanitizers for the same bytes (L45)  {nulla:ratio > X2}
  - [ ] X15.2 the `hb_font_funcs` vtable and `hb-ft` exist to bridge two libraries that both already read `hmtx` (L47)  {nulla:ratio > X2}
  - [ ] X15.3 variation coordinates are set twice and must agree, or text is subtly mispositioned (L49)  {nulla:ratio > X2}
  - [ ] X15.4 hinted vs unhinted advances are negotiated across the boundary (L51)  {nulla:ratio > X2}
  - [ ] X15.5 colour fonts are split down the middle between the two libraries (L53)  {nulla:ratio > X2}
  - [ ] X15.6 there is no shared glyph cache: every application reinvents one above both (L55)  {nulla:ratio > X2}
  - [ ] X15.7 ICU speaks UTF-16: in a UTF-8 codebase every call converts and every returned offset is in the wrong unit (L57)  {nulla:ratio > X2}
- [ ] X16 The missing top layer: none of the three does paragraph layout, so every application writes or imports a fourth library (Pango, SkParagraph) (L59)  {nulla:ratio > X2}

## V. Architecture

- [ ] X17 Eight layers on one substrate: each owns exactly one concern, depends only on layers below it, and is validated by its own oracle (L63)  {DECRETUM}
- [ ] X18 The font parser and the Unicode core are the shared substrate; layout produces a display list that both the screen and PDF consume (L80)  {DECRETUM}
- [ ] X19 The display list is the contract between layout and everything downstream: design it before the PDF writer and the screen renderer, with a serialization stable enough to diff (L229)  {DECRETUM}
- [ ] X20 Two siblings beside the stack, sharing the generators but not the engine: collation and calendars, and font subsetting (L93)  {DECRETUM}
  - [ ] X20.1 the subsetter never prunes GSUB/GPOS, "because PDF viewers do not run layout" (L93, L196)  {DECRETUM}
- [ ] X21 Everything C89 in one tree, unity-buildable, arena-allocated, with no allocation in the shaping or rasterization hot paths; the monorepo's Latin naming applies (L95, L230)  {REGULA > R2}

## VI. The string type and byte offsets

- [ ] X22 "The problem with `char*` was never Unicode … The problem is NUL termination": no O(1) length, no substrings without copying, every function rescans (L101)  {PRINCIPIUM}
  - [ ] X22.1 Rust, Go, Zig, SQLite, MuPDF, SDS and Muratori's `{size, data}` all converged on length-and-pointer UTF-8 (L101)  {nulla:exemplum > X23}
  - [ ] X22.2 UTF-16 is "a 1990s UCS-2 decision nobody would repeat"; Swift's grapheme `Character` made everything O(n), and Swift quietly moved to UTF-8 with byte offsets underneath (L101)  {nulla:exemplum > X23}
- [ ] X23 The string type is a two-word struct holding UTF-8; the struct stays dumb, and byte offsets are "the one currency every layer speaks" (L99)  {DECRETUM}
- [ ] X24 Mix `char*`, code-point indices and UTF-16 "and the project spends its life off by one" (L103)  {PRINCIPIUM}
  - [ ] X24.1 HarfBuzz tags each output glyph with a cluster value back into the text; if that is a byte offset into the slice type every layer uses, no layer translates (L103)  {nulla:ratio > X23}
- [ ] X25 Validate UTF-8 once where bytes enter, and never again; no flag in the struct: "either the invariant holds past the boundary or there are two types" (L108)  {DECRETUM}
- [ ] X26 Cheap names mean cheap work: `str_length` is bytes; Unicode-aware versions get distinct names so the cost is visible at the call site (L110)  {DECRETUM}
- [ ] X27 Store text as typed, unnormalized, so it round-trips byte-exact; normalize to NFC only for comparison, search keys and identifiers (L111, L237)  {DECRETUM}
- [ ] X28 Three tiers of string library: table-free basics, generated Unicode tables, and locale as a sibling library (L114)  {DECRETUM}
- [ ] X29 Interned strings are the one other string type that earns its keep (L112)  {SUPELLEX}

## VII. PDF on the same display list

- [ ] X30 The shared display list is MuPDF's `fitz` architecture, "the strongest single argument for one tree" (L118)  {nulla:exemplum > X18}
- [ ] X31 Subsetting after shaping, from the exact glyph ids the shaper emitted, ligatures and marks included, which no codepoint-based subsetter can know to keep (L122)  {VISIO-OPERIS}
- [ ] X32 Exact ToUnicode, because the shaper's cluster values survive to the writer; third-party stacks lose this at the library boundary (L123)  {VISIO-OPERIS}
- [ ] X33 Print and screen share one layout run: "WYSIWYG is a consequence, not a feature" (L124)  {VISIO-OPERIS}
- [ ] X34 Structure comes free: tagged PDF, PDF/UA and PDF/A from what layout already knows; print requirements become writer modes (L126)  {VISIO-OPERIS}
- [ ] X35 Private data round-trips: alignment, apparatus notes and annotation anchors are embedded, so a PDF reopens with its structure rather than as flat glyphs (L127)  {VISIO-OPERIS}
- [ ] X36 PDF fonts are hostile (Type1, bare CFF, subsets with no `cmap`, Type3); every viewer accumulates font-loading hacks; an in-house parser exposes glyph-by-name and glyph-by-id directly (L132)  {nulla:ratio > R3}
- [ ] X37 A complete writer; a reader for everything the writer produces plus the common subset of the wild; a documented "not rendered" list: "the difference between finishing and inheriting Ghostscript's issue tracker" (L136)  {DECRETUM}

## VIII. Oracles

- [ ] X38 Each layer has a different kind of oracle (exact, implementation-level, opinion, none), and the kind determines how the layer is developed (L140)  {PRINCIPIUM}
- [ ] X39 Write the harness that replays the oracle before writing the layer; a layer is done when its oracle passes, "not when it looks right" (L140, L241)  {REGULA}
- [ ] X40 Diff against a second implementation wherever one exists: "two independent opinions localize bugs faster than one oracle" (L242)  {REGULA}
- [ ] X41 Every oracle is versioned: pin the tag the corpus came from, so "tests changed" and "code broke" stay distinguishable (L156)  {REGULA}
- [ ] X42 An incumbent's golden output is its opinion: a persistent small difference is not necessarily wrong, and matching or diverging is a design choice to record (L156)  {nulla:ratio > X43}
- [ ] X43 Record every deliberate divergence from an incumbent's behaviour in one file, so a future failing oracle case can be checked against intent (L245)  {DECRETUM}
- [ ] X44 The rasterizer's correctness oracle is the one written in-house: a slow, exact, supersampled reference fill (L149)  {SUPELLEX}

## IX. Reading prior art

- [ ] X45 Read reference implementations for structure and decisions, not to transliterate: the Rust port for "what does this table mean", the C original for "what does the world expect" (L160)  {REGULA}
- [ ] X46 A clean rewrite against the oracles avoids the licence question; test data with restrictive font licences stays outside the shipped tree (L176)  {DECRETUM}

## X. Scope and staging

- [ ] X47 1.0 is a complete text engine for the scripts actually typeset (Latin, Greek, Cyrillic, Hebrew) plus USE; hinting and Indic out (L180)  {DECRETUM}
- [ ] X48 Staged so that each stage produces something usable, and each stage's oracle exists before its code (L180)  {DECRETUM}
- [ ] X49 Deliberately out of 1.0, with reasons (L192)  {DECRETUM}
  - [ ] X49.1 TrueType hinting and the autohinter: high-DPI screens made it matter less; unhinted grayscale with fractional positioning is the modern default (L194)  {DECRETUM}
  - [ ] X49.2 Indic, Myanmar, Khmer shapers: "a decade of Uniscribe-compatibility archaeology" for scripts the projects do not typeset (L195)  {DECRETUM}
  - [ ] X49.3 AAT and Graphite; collation, locale formatting and time zones; rendering wild PDFs in general (L197-199)  {DECRETUM}
- [ ] X50 Each excluded item stays possible later "precisely because its oracle already exists" (L201)  {nulla:ratio > X4}

## XI. Risks

- [ ] X51 Perceptual rendering quality has no oracle: "matches golden bitmaps within tolerance" cannot say whether text looks muddy; retired only by rendering a great deal of text and looking at it (L207)  {PRINCIPIUM+intervention, split}
- [ ] X52 HarfBuzz and FreeType are fast "because of a few structural decisions, not cleverness": sanitize once, coverage digests, no allocation in hot paths, a glyph cache (L211)  {PRINCIPIUM+intervention, split}
  - [ ] X52.1 a dense page is about 3,000 glyphs; at 5 µs each, shaping is 15 ms per page, shaped once and cached (L211)  {nulla:ratio}
- [ ] X53 The UCD/CLDR generator is a first-class tool in the tree, emitting two-level lookup arrays and recording the Unicode version; spec drift becomes a regeneration, not a source change (L215, L235)  {DECRETUM}
- [ ] X54 Scope creep toward "render any PDF" is "the single most likely way to not finish" (L217)  {nulla:ratio > X37}
- [ ] X55 The review burden: a wrong offset in one table parser silently misplaces a mark in one font (L209)  {nulla:ratio}

## XII. Practical guidance

- [ ] X56 Sanitize once at load; afterwards every table access is a typed view over the original bytes: "both the performance boundary and the security boundary" (L225)  {DECRETUM}
- [ ] X57 Separate the face (immutable) from the font instance (size, variation, mode); caches key on the instance (L226)  {DECRETUM}
- [ ] X58 Shape in font units and scale at the end; build coverage digests when the face loads (L227-228)  {SUPELLEX}
- [ ] X59 Fuzz from the first parser; sanitizer builds are the default in development (L243)  {SUPELLEX}
- [ ] X60 Render-and-look sessions on a schedule, with fixed specimen pages on the target displays (L244)  {SUPELLEX}
- [ ] X61 One entry point per layer with an options struct, no global state, no callbacks into the caller; keep the API "small and boring" (L249)  {DECRETUM}
- [ ] X62 Expose internals rather than hide them: "the value of an in-house stack is that the next tool up can reach in" (L250)  {REGULA}

## XIII. Addendum: lapide.org, Brighton, rhubarb

- [ ] X63 The lapide.org corpus contradicts the 1.0 script scope: it already publishes in Arabic, Farsi, Hindi, Tamil, Thai, Chinese and others, and per-language PDFs need their shapers (L276)  {nulla:ratio > X47}
  - [ ] X63.1 Thai, Lao, Khmer and Burmese line breaking is dictionary-based; CJK needs kinsoku rules and per-script font fallback (L286)  {nulla:ratio > X47}
- [ ] X64 Rhubarb already holds more of the substrate than the body assumes (L288)  {nulla:ratio > R3}
  - [ ] X64.1 `chorda` is already the string type; Unicode tables (Tier 2) are missing entirely, "the true first stage" (L294-295)  {nulla:ratio}
  - [ ] X64.2 the golden-bitmap harness exists (`imago_collatio`, `specimen`); identity exists (`sigillum`, `mintid`); `paginatio` is the text-to-page mapping Brighton wants (L298-302)  {OCCASIO}
- [ ] X65 The sentence segmenter is load-bearing for identity: change one rule and every sentence id in the archive changes (L306)  {PRINCIPIUM}
  - [ ] X65.1 so its rules should be UAX #29 plus a documented abbreviation list, frozen or versioned per text, and the key normalization written down once (L306)  {DECRETUM}
- [ ] X66 Brighton is web-first, so the stack's first real consumer is PDF and print, not the screen (L310)  {nulla:ratio > X67}
- [ ] X67 Reordered staging: Unicode core, parser, shaper, layout, then subsetter and PDF writer as the first deliverable; the rasterizer moves from stage 3 to stage 6 "with nothing lost" (L310-321)  {DECRETUM}
- [ ] X68 A publisher's PDF writer needs what the body under-weights: hyphenation, Knuth-Plass line breaking for print, a page-layout tier with footnotes, running heads and marginal apparatus ("the classical Glossa Ordinaria layout problem"), and printed indices (L325-328)  {VISIO-OPERIS}
- [ ] X69 Scanned PDFs encode pages as JBIG2 or JPX: extract them once, offline, to PNG or JPEG into the `.brighton` file, so the reader only meets images it already decodes (L332)  {DECRETUM}

## XIV. What the text leaves open

- [ ] X70 The 1.0 script scope: keep it, pull Arabic and USE forward, or let the corpus decide (Indic too); "none chosen here" (L278-284)  {nulla:quaestio}
- [ ] X71 The exact key normalization (NFC, whitespace, quote style), since polytonic Greek and pointed Hebrew have several valid encodings of the same visible text (L306)  {nulla:quaestio}

## XV. Since then (my commentary, not in the source)

- X2 ("a single problem split by history") looks like an instance of tabularium's T1 (toolchains re-derive the world through lossy handoffs): the same condition at library scale. A lineage candidate: X2 under T1.
- The Unicode core has partly been built since: `runae` (width, graphemes, drawable units) reached main 2026-09-29, and the ICU, HarfBuzz and FreeType trees are cloned beside the repo as references (2026-09-28).
- Lapide-v2 hashes the *printed* (diplomatic) Latin, not normalized text (strata-textus.md §3.5), so a change of segmentation or normalization rules no longer re-mints addresses wholesale: X65 and X71 were partly answered by moving identity to a different layer.
- X21 mostly restates house regulae (C89, arenas, Latin names) rather than adding new ones.
