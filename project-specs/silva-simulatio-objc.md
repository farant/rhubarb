# Simulatio: stratum ObjC silvae (rung 1 + censor + illuminatio speculi)

2026-07-21, post arcum amalgamatis. Scope simulated: lexer rung
(`@`), censor-on-.m, speculum bridge highlighting. Desideratum:
01KY39VJY5 (three consumers). Grounding = REAL (census + reads),
implementation = pretend. Format: four-part complexity schema.

## Real grounding facts (verified, not simulated)

- CORPUS: 4 files, 2,514 lines. tls_macos.m = ZERO @-constructs
  (ObjC by extension only); clipboard = 2 @autoreleasepool. The real
  @-surface lives in fenestra (8 @interface/@impl/@end, 7 @selector,
  15 @", 18 @autoreleasepool/@try, 15 methods) + vitrea (12, 0, 8,
  5, 5). ZERO blocks corpus-wide (matches decretum 01KXZZDPNK:
  block literals forbidden). One suspect @{ hit in vitrea
  (verify before fixture-pinning — likely inside a JS string).
- `@` today = SILVA_LEX_OCTETUS_IGNOTUS (one byte, roundtrips) —
  total-lexer guarantee means .m fidelity bar is pinnable BEFORE
  any change.
- #import: 9 sites, all SDK frameworks; expander already passes
  unmatched directives through byte-conservatively → opaque-#import
  is CORRECT v1 behavior (house-dialect opacity), not a gap.
- The .m files are LATIN (79 si/redde/dum sites in fenestra alone).
- speculum.js highlighter already accepts .m (illuminandum_est) with
  degraded C-shaped rendering; per-line char-walk, comment state
  carried across lines; classes com/cho/cla/lat/pra.
- Wearers link ZERO silva (compile_tests fontes list) — see SO4.
- 96 lexeme genera currently.

## Complications register

**SO1 — two-token @, not composite lexemes.**
Discovered-while: sketching the genus. Consists-in: compositing
@interface into one lexeme hides the directive identifier from the
expander, which legally operates on it. Consequences: lexer delta =
ONE genus (SILVA_LEX_AD, bare @) + ~10 lines; @"..." = AD + CHORDA;
expandere/conditio untouched. Handled-by: grammar rung composites
via productions (P1 keyword-genera pattern) when it comes.

**SO2 — genus enum extension = ABI ceremony.**
Discovered-while: adding the genus. Consists-in: SILVA_LEX_* is
CADENDA-mirrored in silva.h and baked in tables; renumbering
poisons objects (M2c scar). Handled-by: append-only at tail +
amalgam regen; the newest_header guards (2026-07-21 hygiene) now
cover all three drivers.

**SO3 — censor-on-.m v1 = lexical tier, honestly labeled.**
Discovered-while: pointing censor at fenestra. Consists-in: message
sends parse as broken C under the c89 grammar → ERROR nodes → tree-
adjacent checks void. Consequences: [SINE ARBORE] degraded mode is
the DESIGNED v1 behavior; discharges 01KXZYG2SE's censor half.

**SO4 — THE FINDING: wearers carry no silva; the lexer is separable.**
Discovered-while: sizing the bridge's native side. Consists-in:
highlighting needs silva_lexare in the WEARER; whole amalgam =
absurd; but the amalgamator mech build proves silva_token.o +
silva_lexema.o stand alone over piscina/chorda (already linked).
Consequences: +2 objects per wearer; FIRST lib/-consumes-silva/
layering precedent. OPEN FOR FRAN: is lib/speculum.c → silva/fontes
dependency acceptable, or does the lexer pair graduate to lib/
(speculum's own "component graduation" future)?

**SO5 — the JS fallback highlighter cannot die.**
Discovered-while: sketching span rendering. Consists-in:
probatio_velaminis.js runs real speculum.js in JavaScriptCore with
NO bridge; headless path needs a highlighter. Consequences: two
highlighters (one-truth smell). Handled-by: regex walk FROZEN as
fallback (degraded .m acceptable there); velamen probatio gains a
fake-bridge fixture with canned spans so the span path is headless-
tested too.

**SO6 — the rung-1 dialect is smaller than sketched.**
Consists-in: census shows the four files need NOTHING beyond AD +
existing lexemes at the lexical level — no blocks, no modern
literals (pending the one @{ verification), methods/message-sends
lexically ordinary. Consequences: rung-1 estimate SHRANK under
simulation.

**SO7 — aedilis/fidelity interactions are rung-2 business.**
Consists-in: the .m -MM oracle lane retires only when trees exist
(connects to parked 01KY01QF8E closure-sine-MM); rung 1's side
effect = percursus/infidelis sweep .m as lexically-clean, fidelity
corpus +4 files. Pin in RELATIO when real.

## Verdict

Rung 1 + censor + bridge highlighting = one small arc, no engine
work, no surprises at the lexer/preprocessor layers (the total-
lexer and byte-conservative-directive designs absorb ObjC
gracefully — the maximalist pins paying out again). Two decisions
for Fran before building: SO4 (layering: wearer links silva/fontes
objects vs lexer graduation) and SO5's fallback freeze. One
verification owed: the vitrea @{ census hit.
