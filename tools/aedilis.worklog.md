# aedilis worklog

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
