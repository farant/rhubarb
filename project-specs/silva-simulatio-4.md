# Silva Simulatio ④ — Amalgamation + Prefix-Rename

*Implementation simulation (2026-07-02), ~1.5 simulated hours — except most of it
wasn't simulated: this sim's centerpiece was a REAL crude amalgamation of the seven
vendored libraries, actually compiled under the full rhubarb flag set, plus a REAL
~40-line token-aware renamer prototype, actually applied and recompiled, plus REAL
symbol-collision measurement against rhubarb's own build objects. Artifacts in the
session scratchpad: amalgama_probatio.py, amalgama.c, amalgama_renamed.c.
Complications S41–S47 continue the register in the Speculum complexity schema.
This is the last of the four M0 simulations.*

## The experimental results

1. **The seven libraries amalgamate with ZERO source changes.** One 7,895-line
   translation unit (latina.h + 7 headers + 7 bodies, project includes stripped,
   system includes hoisted) compiles clean, first try after header-order fix,
   under `-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings`.
2. **Zero cross-file duplicate static function names** (27 statics inventoried).
   The duplicate-static fear is measured away.
3. **Zero `##`/`#` operators in all fourteen files** — confirmed, not assumed.
4. **Collision risk is real and measured**: 155 external defined symbols in the
   amalgam; 15/44/32 of them collide with rhubarb's own piscina.o/chorda.o/xar.o.
5. **The token-aware renamer works and is small**: ~40 lines (walk source, skip
   comments/strings/chars, rename identifiers on exact-token match against the
   prefix map). It renamed **1,084 occurrences**; the renamed amalgam compiles
   clean under the full flag set; shared external symbols with rhubarb's objects
   drop to **zero**.
6. System includes across all seven: just `ctype.h stdio.h stdlib.h string.h`.
7. latina.h measured: 422 lines, 388 #defines including the keyword macros
   (`si`, `alioquin`, `per`, `dum`, `redde`, `character`, `integer`, `nomen`).

## Complication register

### S41 — duplicate statics: dissolved by measurement
- **discovered-while:** inventorying static functions across the seven bodies
  before concatenating (the classic amalgamation failure mode).
- **consists-in:** two files with a same-named static helper become a duplicate
  definition in one translation unit.
- **consequences:** none — the count is zero across all seven libraries.
- **handled-by:** nothing needed now; the amalgamator still detects-and-errors on
  future duplicates (cheap check during assembly) so the property is enforced,
  not assumed.

### S42 — header topology ≠ body topology
- **discovered-while:** the first compile failed exactly as the include-graph
  predicted: friatio.h and chorda_aedificator.h both need chorda.h, so the
  downstream report's single ordering (friatio before chorda) breaks at the
  declaration level.
- **consists-in:** the chorda⇄aedificator cycle exists only at the BODY level
  (chorda.c uses the builder); at the header level the graph is acyclic with a
  different order. One ordering cannot serve both.
- **consequences:** the amalgamator needs two topological sorts: headers
  (latina → piscina → chorda → chorda_aedificator → friatio → tabula_dispersa →
  xar → internamentum) and bodies (any order once all decls precede all bodies).
- **handled-by:** decls-first assembly with independent header/body orderings —
  verified by the clean compile. The manifest declares both orders explicitly.

### S43 — silva.h must be vanilla C89 (the host-pollution boundary)
- **discovered-while:** measuring latina.h: 388 #defines including `si`, `per`,
  `character` — inlined into the amalgam body, which is fine for silva.c, but a
  host including a silva.h that pulled latina would have `si` rewritten to `if`
  in its own code.
- **consists-in:** the public boundary and the internal idiom have incompatible
  lexical needs; any latina leakage through silva.h is hostile to the host.
- **consequences:** silva.h is written in plain C89 spellings (int, long, char*,
  unsigned; silva_-prefixed struct/typedef names); latina.h is inlined into
  silva.c AFTER the public-header section, so internal code keeps the idiom.
  Public API signatures never use latina type aliases.
- **handled-by:** amalgamator assembly order inside silva.c: [system includes]
  [silva.h verbatim] [latina.h] [renamed internal decls] [bodies]. A generated
  test compiles a host file that includes silva.h and declares variables named
  `si`, `per`, `character` — must compile clean.

### S44 — static internalization × -Werror: the unused-function trap
- **discovered-while:** planning the belt-and-suspenders step (rename for
  legibility + `hic_manens` injection so internals vanish from the export
  surface entirely, shrinking 155 externals to just the public API).
- **consists-in:** once internal functions are static, every UNUSED one (all of
  sha1_*/sha256_*/crc32_* — their tables CRC32_TABULA/SHA256_K are visibly
  compiled into the object today) becomes -Wunused-function → -Werror → the
  amalgam fails to compile.
- **consequences:** static internalization forces function-level subsetting; the
  manifest must say which functions ship, not just which files. friatio needs
  only fnv1a/fnv1a_literis/djb2 (confirmed by the earlier inventory).
- **handled-by:** manifest lists per-file exclusions (friatio: keep 3 functions);
  the amalgamator drops excluded function definitions and their prototypes; a
  post-assembly compile in the test suite enforces zero unused. Static injection
  must rewrite BOTH the decl-block prototype and the definition (mismatched
  static/extern declarations are themselves errors).

### S45 — the renamer is smaller than feared
- **discovered-while:** prototyping the "haunted" text-level rename that sim ④
  existed to de-risk.
- **consists-in:** the risk was renames-in-strings, partial matches (lowercase
  `chorda` inside `chorda_fissio_fructus`), and Latin prose collisions in
  comments.
- **consequences:** with a real tokenizer pass (skip comments/strings/chars,
  exact identifier matching, longest-prefix function map + exact type map) the
  entire hazard class vanishes: 1,084 renames, clean recompile, zero residual
  collisions. The C89 amalgamator version is a few hundred lines, most of it the
  tokenizer silva's own lexer will eventually replace.
- **handled-by:** the prototype's algorithm carries directly into
  silva/instrumenta; the collision check (nm intersection against host objects)
  becomes a scripted test; identifier-boundary renaming is CONFIRMED sufficient —
  no token-pasting or stringization exists to defeat it (measured, S41 script).

### S46 — include guards can just stay
- **discovered-while:** assembling — the plan said "strip guards / uniquify";
  leaving them in was simpler for the crude pass.
- **consists-in:** inlined headers' `#ifndef X_H` guards are harmless in a single
  file (each fires once) and cost nothing.
- **consequences:** one less transformation in the amalgamator; the guard macros
  (PISCINA_H etc.) do land in the host's macro namespace if it later includes
  the real headers — which is exactly the double-inclusion protection wanted.
- **handled-by:** keep guards verbatim. (Revisit only if a host complains about
  the macro names themselves.)

### S47 — C89 external-identifier length (footnote)
- **discovered-while:** renaming produced silva_tabula_dispersa_iterator_proximum
  (39 chars) — beyond C89's guaranteed-significant 31 for external names (6 for
  truly ancient linkers).
- **consists-in:** strictly, a maximally-pedantic C89 linker could conflate long
  externals.
- **consequences:** none on any platform that matters (clang -pedantic accepted
  it silently); static internalization (S44) moves nearly all long names to
  internal linkage (31 significant chars, satisfied) anyway.
- **handled-by:** noted as a portability footnote; the handful of PUBLIC silva_*
  API names are kept under 31 characters as a naming rule.

## Design changes to fold into spec-v2

1. Amalgamator manifest: two explicit orderings (headers/bodies) + per-file
   function exclusion lists (S42, S44).
2. silva.c assembly order: [system includes][silva.h][latina.h][internal decls,
   renamed+static][bodies] with the silva.h vanilla-C89 boundary rule and the
   host-pollution test (S43).
3. Static internalization is load-bearing (export surface = public API only);
   prefix-rename retained for debugger/profiler legibility per the original
   interview decision — both applied (S44, S45).
4. Duplicate-static detection and the nm-intersection collision check become
   amalgamator-suite tests (S41, S45).
5. Include guards pass through verbatim (S46). Public API names ≤ 31 chars (S47).

## Open questions for Fran — ANSWERED 2026-07-02

*Interview transcript: silva-simulatio-4-interview.md. Resolutions folded into
silva-spec-v2.md §11. M0 CLOSED.*

1. friatio: **manifest function exclusions** — canonical lib untouched;
   mechanism generalizes.
2. silva.h: **vanilla C89 boundary confirmed**, docs included; host-pollution
   test enforces permanently.
3. Guards: **keep verbatim** — shared macro names are intended double-inclusion
   protection.

## Verdict

The cheapest simulation returned the strongest result: **the deliverable
mechanism is real.** A crude script produced a compiling single-file library
from the actual seven vendored sources in one afternoon-equivalent, the renamer
that was feared haunted is ~40 lines and verified collision-free, and the two
genuine discoveries (header/body topology split S42, the static×-Werror unused
trap S44) both have mechanical resolutions. M0 is COMPLETE: all four simulations
run, all registers resolved or pending one light interview. Next: fold, then the
walking skeleton — real code in silva/.
