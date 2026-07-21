# aedilis worklog

## 2026-07-20 — Phase A (the engine) — sealed

**Shipped**: include/aedilis.h + lib/aedilis.c (the closure engine),
tools/aedilis.c rewritten as the CLI (silva extractor + -MM lane +
git provenance + manifest emission), aedilis.stml (repo config,
committed, uses the day-old raw-capture syntax), fixture mini-repo
under probationes/fixa/aedilis/, probatio_aedilis engine sections
(config/derivation/variants/refusals/manifest — all green first
run), compile_tests SOURCE_FILES += lib/aedilis.c (additive lib
registration, speculum precedent — NOT the suite-adoption decision,
which stays deferred).

**The load-bearing structure**: the engine is PURE — extraction is
injected via the AedilisExtractor seam (vitrea_missor precedent).
silva + clang -MM live only in bin/aedilis; the probatio drives the
engine with a table extractor over REAL disk fixtures (existence
probes are genuine, directives are faked). Engine deps: via, filum,
stml, tabula_dispersa, chorda — no silva.

**Config = aedilis.stml, radix = its dirname** — so the fixture
config IS a complete self-describing mini-repo and the real one
roots at ".". Sections: inclusa (plain captures), vexilla (raw
captures), variantes praelatio (ordered fallback words), nexus
regulae (per-object frameworks), vendor (per-fons flags),
irregularia (caput → obiecta).

**Variant probe order** (survey-derived, not spec-assumed): for
caput X.h, per praelatio word V: lib/X_V.m, lib/X_V.c; then base
lib/X.c. The decisive real case: praelatio "macos posix" finds
tcp_posix.c on macOS (no tcp_macos exists) — praelatio is a
FALLBACK CHAIN, not a single platform word. --varians V narrows
the chain to [V] + base.

**Live results**: lib/hospitium.c → 14 objects/126ms with
tls_macos.m carrying -framework Security; probatio_vitrea_hospes.c
→ 28 objects/213ms with all three frameworks on the right objects,
origo=oraculum on the .m-discovered entries, and the speculum
capsula via a REAL annotation now in the wearer (first live
annotation in the tree).

**Decisions made in-phase (Fran may veto)**:
1. Unresolved include = SYSTEMA, never refusal — angle/quoted are
   indistinguishable (silva strips the form, spike finding), so a
   typo'd house include classifies as system; it fails loud at
   compile and differentia (Phase B) flags it. est_angulata
   (01KY118F) would upgrade this to a refusal for quoted misses.
2. ANNOTATED objects are link-declared, NEVER WALKED — the wearer's
   generated 9.2MB capsula parsed for 17s before this rule; deps of
   generated objects belong to their generators. (absens objects
   were already unwalkable; this makes the semantics uniform.)
3. Vendor is OPAQUE — vendor/X.h → vendor/X.c convention probe,
   flags from rules, never walked (sqlite 8MB, stb single-header).
   Header-only vendor (stb) lands in capita, unwalked.
4. fabrica.tsv channel DEFERRED — its obiectum species is .o-name
   → source (excubitor's staleness domain), not caput → obiectum;
   aedilis channels are aedilis.stml + annotations; fabrica
   absorption remains the later milestone (interview Q8).
5. origo vocabulary: "configuratio" (not "aedilis.stml") for
   irregular-rule provenance — attribute-value cleanliness.
6. Engine walks house HEADERS too (transitive closure is the
   point); vendor and annotated objects are the only unwalked
   reachables.

**Gotchas**: -MM emits the source itself bare while the engine
passes "./"-joined paths — self-skip must SUFFIX-match (first
wearer run had lib/tls_macos.m as a phantom caput). stml lacks
textum_addere_ex_chorda — creare_ex_chorda + liberum_addere.
argumenta_parsere wants i32 argc (examen push caught the s32).
gesta scope (tabularium.h, -Igesta/fontes) not yet resolvable —
Phase C extends inclusa or scopes config when the gesta consumer
lands.

## 2026-07-20 — Phase 0 spike (sealed)

Spec of record: project-specs/aedilis-spec-v2.md (implement from it,
never from a summary). Park: 01KXJ2HV.

**Gate: PASSED.** `bin/aedilis lib/hospitium.c` prints exactly the 13
known includes (11 quoted + 2 angle), source order, matching both
`grep '#include'` and legati inclusiones. successus=1 errores=0.

**Shipped**: tools/aedilis_struere.sh (dedicated build — nexus.sh
mold; generic tools loop can't supply -Isilva/amalgama), minimal
tools/aedilis.c (parse one file → directives + annotations +
timings), probationes/probatio_aedilis.c (via + stml specimen pins,
44 asserts, suite-discovered automatically — compile_tests.sh
untouched), probationes/fixa/aedilis/annotatum.c (annotation
fixture).

**Spike findings (the reasons this phase existed):**

1. **stml attribute escaping is ABSENT — and asymmetric by design.**
   Every writer path emits attribute values RAW inside `="..."`;
   the reader scans to the closing quote and decodes no entities
   (stml.h: "No entity references"). Consequences, pinned in
   probatio_aedilis: values round-trip VERBATIM including
   `< > & ' =` and spaces; a double quote `"` is UNREPRESENTABLE
   (raw write → malformed document → value does not survive).
   Phase A: aedilis REFUSES LOUD on `"` in any attribute value.
   (CORRECTION same day, capture specimens: TEXT nodes are
   SYMMETRIC — `_scribere_evasus` escapes and `_unescape_entities`
   (stml.c:1377, text-parser-only call site) decodes on read, so
   element text is value-preserving for all five entities. The
   "No entity references" comment in stml.h is stale. Attributes
   remain the raw-no-decode lane.)

## 2026-07-20 — capture specimens (Fran's question, post-seal)

Scratch specimen (JXA-of-C-style: scratchpad program against
build/*.o) answered "can forward capture replace attributes for
quote-bearing values":
- `<via (>value` (plain capture): parses, captures to END OF LINE
  as a text child, pretty round-trip BYTE-STABLE. Text semantics:
  entity-escaped on write, decoded on read → value-preserving,
  but `&quot;` noise appears in the emitted file.
- `<tag!>anything <&"> here</tag>` (raw closed): fully VERBATIM
  both directions, round-trip stable — the true CDATA lane. Only
  unrepresentable content: the literal closing-tag text itself.
- `<tag! (>` (raw + capture): the WRITER can emit it, the READER
  rejects it (STML_ERROR_TAG_NON_CLAUSUM — raw scanner wants a
  closing tag). Writer/reader asymmetry; desideratum filed.
Design consequence for Phase A: manifests (machine-emitted) =
attributes for identity/provenance (refuse `"` there) + raw-closed
`<vexillum!>` children for flag units (one unit per element —
`-framework Cocoa` stays one line, quotes legal, emitter
interpolates without re-tokenizing). aedilis.stml (hand-written) =
captures welcome as authoring sugar; raw captures once the reader
learns them.
2. **SilvaInclusioVista strips the include form**: `<stdio.h>`
   arrives as bare `stdio.h`, indistinguishable from a quoted
   include. Phase A resolution-by-probe (includer-relative → -I
   list → else system) handles both correctly under house
   conventions; differentia-clausurae confirms corpus-wide.
   Desideratum filed for an `est_angulata` field silva-side.
3. **Annotation grammar must be ANCHORED, not substring.** The
   fixture's own header comment *quoting* "aedilis:" in prose was
   captured by the first substring scan (the fixture caught
   itself). Rule now: after the comment delimiter and whitespace,
   the comment must BEGIN with `aedilis:`. Phase A inherits this.
4. **Costs (open items 1+2 closed).** Amalgam silva.o: 0.3s at
   -O0, 2.4s at -O2 with FULL house flags (celer precedent) —
   cached thereafter; struere uses -O2. Parse via
   silva_c89_parsare_cum_contextu + latina lexicon: 1.108 ms/KB at
   -O0, **0.478 ms/KB at -O2** (hospitium.c 31.8KB → 15.2ms) — in
   line with the phase-log celer figure; fresh-parse decision holds
   with room to spare. Small-file floor ≈ 5ms/parse (latina lexicon
   parsed per call) — Phase A knob if closure walks ever care:
   reuse one SilvaExpansio via silva_parsare_cum_expansione.
5. **lexare_cruda comment scan**: 8098 lexemata / 0.8ms on
   hospitium.c; comment tokens carry delimiters in valor.

**Plumbing gotchas**: chorda.c/filum.c pull chorda_aedificator.o
(link set is piscina, chorda, chorda_aedificator, filum, silva.o);
`echo ===` dies in zsh (equals-expansion — use plain words);
annotation filter compares (i8) casts byte-wise since SilvaChorda
is not house chorda.
