# CSS / HTML / JS — the web-format parsing libraries

*2026-08-18. Decided with Fran: three general-purpose, reusable parsing
libraries in the root tier (the git-library model: first consumer is
modest, design outlives it). First consumer: syntax highlighting in the
laboratorium lector (the `data-color` derived-layer seam, 0023).
Long-term: these are browser-adjacent groundwork — vitrea serves these
formats today with no ability to read them.*

Order of construction: **CSS → HTML → JS** (ascending difficulty; CSS's
spec-defined error recovery makes it the cheapest place to establish the
shared contract, which the others then inherit).

## The shared contract (silva's bar, signed by all three)

1. **Total lexing.** Any byte sequence lexes to a token stream. Nothing
   is ever dropped, nothing ever crashes. Garbage and truncation get
   NAMED genera (`…_IMPERFECTA` for unterminated strings/comments/
   templates/urls; `IGNOTUM` for untokenizable bytes).
2. **Byte fidelity.** `emittere(lexare(x)) == x` for arbitrary input.
   Token values are chordae pointing INTO the source (fons non copiatur
   — silva's rule); emission is concatenation. Enforced as a property
   gate over a real corpus (probationes/fixa/<lingua>/ — snapshots of
   house files plus adversarial fixtures).
3. **Positions in bytes.** offset + mensura in octets, linea/columna
   1-based. This is decretum 01M0ATF1E1's language; consumers converting
   to UTF-16 (JS-side) convert at the boundary.
4. **Trivia are tokens.** Whitespace and comments are emitted in-stream
   (the CSS spec skips comments; we deviate — fidelity and the comment
   query layer outrank tokenizer-spec conformance; a spec-conformant
   consumer filters them).
5. **Verbatim spans now, cooked values later.** The lexema stage does
   not decode escapes/entities/references into clean values; it only
   understands them well enough to find token boundaries. Cooked values
   (CSS escape decoding, HTML character references, JS string values)
   are an arbor-stage concern, following silva's valor-mundus pattern.
6. **Piscina in, Xar out.** No hidden allocation, no global state.
   Tokens by value in the Xar (no origo chains here — these languages
   have no preprocessor).
7. **Staged shipping.** `<lingua>_lexema` ships first (highlighting and
   the roundtrip gate prove it). `<lingua>_arbor` follows per-language
   when a consumer wants trees. A lexema module never grows tree
   knowledge.

## Naming and layout

Format names stay untranslated technical terms (house precedent: json,
stml, http, toml, url): `css_lexema.{h,c}`, `html_lexema.{h,c}`,
`js_lexema.{h,c}` in root include/ + lib/, managed by silex renovare
(so the laboratorium vendors them properly — unlike the manually-copied
silva lexer of 0023, which stays a one-off). Tests:
`probationes/probatio_<lingua>_lexema.c`. Genus enums use Latin token
names (CSS_LEX_APUD_VERBUM for at-keyword, etc.).

## Per-language scope

### CSS (css_lexema — this slice)

CSS Syntax Module Level 3 tokenizer, the full token vocabulary:
identificator, functio (ident+paren), apud-verbum (@media), cancellum
(#hash), chorda / chorda-imperfecta (bad-string — SPEC-DEFINED
recovery), url / url-imperfecta (bad-url), numerus, centesima,
dimensio, spatia, commentarium / commentarium-imperfectum, CDO/CDC,
punctuation, delim, finis.

Deliberate deviations from the spec tokenizer, all in fidelity's favor:
- **No input preprocessing.** The spec normalizes \r\n→\n, \f→\n,
  NUL→U+FFFD before tokenizing; that would break roundtrip. We lex raw
  bytes (\r and \f are whitespace characters directly; NUL falls to
  delim). Value normalization is an arbor concern.
- **Comments are emitted**, not skipped (contract §4).
- **EOF-unterminated url → url-imperfecta** (spec returns a valid
  url-token on EOF; we prefer loud).

Bytes ≥ 0x80 are name characters (correct for UTF-8: all continuation
bytes are ≥ 0x80).

Later (css_arbor, when a consumer wants it): component values, rules,
declarations, with the spec's recovery algorithm. NOT in scope ever at
this layer: cascade/specificity/computed values (consumers), a CSS
selector-matching engine (selectio exists for stml; CSS matching is its
own future decision).

### HTML (html_lexema, then a SIMPLE arbor)

Tokenizer: the tractable half of HTML5 — data / tag-open / attribute
name & value (quoted, unquoted, single-quoted) / comments / doctype /
raw text (script, style) / RCDATA (title, textarea). Case-insensitivity
recorded, not normalized (verbatim spans).

Arbor stage: a simple nesting builder with void-element knowledge and
basic implied-close recovery. **Deferred by name: the full HTML5
tree-construction algorithm** (insertion modes, adoption agency,
foster parenting) — the path back is "replace the simple builder,
lexema untouched." HTML is NOT stml: unquoted attrs, void elements,
case games, raw-text elements. Separate lexer, no code sharing.

### JS (js_lexema, then js_arbor)

Target: **the house dialect** — ES5 plus the ES2015 the repo actually
writes: class, arrow functions, template literals, let/const. The
corpus (componentia.js, laboratorium.js, scholar files) must lex AND
eventually parse. Fran's direction: house dialect now, more robustness
later — so total lexing means later-ES syntax still LEXES cleanly
(tokens are mostly version-agnostic), and unknown constructs at the
arbor stage become named IGNOTUM nodes, not errors. Growth = teaching
the arbor new forms, never re-lexing.

Lexer hazards owned from day one: regex-literal vs division (previous-
significant-token heuristic), template literals with nested
interpolation (a lexer-stack), line-terminator awareness (ASI is an
arbor concern but the lexer must record newline trivia for it).
Sputnik's lexer/parser is the skeleton to fork (Pratt + uniform AST,
arena, comments preserved) — sputnik itself stays untouched (its
kebab-case identifiers and Latin keywords are its own language).

## Consumers, planned

1. **Coloring pipae** (lab experiments, one per language): spans + Latin
   coloring policy, byte columns, version stamps — the 0023 template,
   lector unchanged.
2. **js_syntaxis gate, rebuilt**: today's gate is macOS-only JXA
   (`new Function`) yielding yes/no. js_lexema+arbor gives a pure-C89
   gate with positioned diagnostics — and Linux portability.
3. **Theming tools**: css_lexema over thema.css / componentia.css —
   variable inventory, `--ca-*` audit, harmonia applicare-thema.
4. Further out: fragmenta lint (lab doctrine checks), capsula asset
   analysis, formatters, the new-internet reading stack.

## Testing doctrine

Per-module probatio (token-stream cases, every IMPERFECTA genus
exercised, position spot-checks) + the corpus roundtrip gate: every
fixture in probationes/fixa/<lingua>/ must survive
`emittere(lexare(x)) == x` byte-compare; fixtures include snapshots of
real house files AND adversarial files (unterminated everything,
mixed line endings, non-ASCII, empty file). A NUL-byte case rides in
the probatio inline (not as a fixture file).
