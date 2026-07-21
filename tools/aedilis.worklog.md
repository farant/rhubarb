# aedilis worklog

## 2026-07-21 — the `corpus` verb: headers declare their own bodies

Fran's instinct, and the taxonomy's actual answer: "which files
implement me" is a SITE-LOCAL fact about the header, so it belongs
in the header — the irregularia went to config only because the
gate discovered them via link failures and config was the fastest
channel. First engine change since Phase B, and a small one:

- New annotation verb for headers: `aedilis: corpus lib/x.c` —
  new origo AEDILIS_ORIGO_CORPUS. Semantics vs `obiectum`:
  corpus objects are WALKED (their includes join the closure;
  1MB cap still protects — biblia_dr/glr_tabula ride it), and an
  absent corpus is a REFUSAL (a header claiming a missing
  implementer is a rotted annotation, not a pending generation).
  caput field = the annotating file (self-documenting manifests).
- Annotations COMPOSE with the convention probe instead of
  replacing it (irregulare replaced probe wholesale) — so most
  rules shrank to their genuinely exceptional part: fenestra
  needs only the fenestra_textus line (probe still finds
  fenestra_macos.m by variant), nuntium_schema only the generare
  line, arbor2_glr only the tabula line, calendarium only
  sanctorale. Nine annotation lines across eight headers replaced
  ~20 config lines; irregularia section now EMPTY (mechanism kept
  for unowned headers / future per-variant splits).
- Migration proven semantics-preserving: --enumerare closures for
  tempus/nuntium_rotunda/libri byte-identical before vs after.
- The verb name deliberately echoes legati's corpus tool: the
  header is the declaration, the corpus is where the body lives —
  two tools, one word, one meaning.
- Differentia symmetry holds free: corpus objects are walked in
  the silva lane AND -MM'd in the union (the skip only exempts
  ANNOTATIO). Known pre-existing micro-asymmetry: cap-skipped
  objects still get -MM'd in the union — live-proven harmless
  (their headers are already in capita via other paths).

## 2026-07-21 — the Tier-1 batch: 23 launchers source derived truth

All tool launchers converted in three subtree passes (silva 12,
officina 7, loners: saltuarius/spectaculum/gesta-tabularium) —
each sources a committed <basis>_fontes_generata.sh; regeneration
line rides in each snippet header. Remaining census entries are
deliberately out: 5 compile_probationes suite runners (Tier 3
family) and 3 amalgamare (Tier 2's emitter).

**The load-bearing lesson**: a launcher's derivation targets are
NOT just its principal — they're everything the script compiles
against the lib set (silva fontes modules for arbor/auspex/
lustrum/generare, instrumenta modules for legatus/sonda/vindex,
subtree fontes globs for the rest). First-pass snippets from
principals alone under-derived exactly there; the link said so
loudly, per design.

**Latent break found and fixed**: silva/generare.sh had compiled
instrumenta/nexus_ordines.c since the LEGATUS era without linking
the amalgam it needs — dead since then, unnoticed because grammar
regeneration hadn't run. Named skip added (silva_amalgama
precedent); full regeneration now runs green and IDEMPOTENT
(fontes/ diff empty — determinism re-proven free).

**Two self-inflicted verification lessons**: (1) ran the HEAD
version of a script from /tmp — BASH_SOURCE-relative paths made it
exit early and I read "0 errors" as "HEAD works" (marker-grep
cousin of the exit-code law: run the thing in its true habitat);
(2) fusor "FRACTUM" was my grep matching the corpus sweep's own
output — bare-running an unknown launcher as a smoke launched a
multi-minute sweep (know what a script DOES before bare-running
it; officina/CLAUDE.md had the cheap exemplars all along —
interpretare's fasti smoke → [BENE 13 instructiones]).

Derived lists trimmed hand fat repeatedly (internamentum off
nexus, tabula_dispersa/friatio off arbor-class, etc.). Config
inclusa grew to cover subtree header homes (silva/instrumenta,
officina/fontes, tessera/amalgama+fontes, saltuarius/fontes);
porta re-certified after.

## 2026-07-21 — self-hosting: aedilis_struere.sh derives itself

First of the Tier-1 conversions (29 scripts carry the hand
RADIX_FONTES pattern; census 2026-07-21). New GENERIC generator
tools/fontes_generare.sh (<exitus.sh> <fons.c>... → union of
closures → lib basenames snippet); aedilis_struere.sh now sources
the committed tools/aedilis_fontes_generata.sh — the spec's own
prophecy ("aedilis will one day emit its own") fulfilled. The
bootstrap egg is broken by COMMITTING the snippet: fresh checkout
builds from it; only regeneration needs bin/aedilis.

The derived list matched my hand list exactly (13) — including
selectio, which I originally discovered via a Phase-0 link error
and derivation now finds through stml.c's include. Config gained
silva/amalgama in inclusa so "silva.h" stops classifying as a
system header in the self-manifest; the amalgam then derives as a
real object (adjacent-dir probe) but the 1MB walk-cap keeps it
unparsed and the lib/-only snippet filter keeps the script's
special amalgam handling. Gate re-certified after the config
change. gesta/fontes_generare.sh predates the generic tool and
can migrate to it during the bench batch.

## 2026-07-20 — Phase C (consumers + seal)

**The first hand list is RETIRED — and aedilis's first act was to
trim it.** gesta/fontes_generare.sh derives the union of both
gesta probationes' closures via the new `--enumerare` query mode
and emits gesta/fontes_generata.sh (committed, sourceable);
compile_probationes.sh now sources it instead of carrying the
literal array. The derived list has 17 entries where the hand
list had 18: **utf8 was fat** — nothing in the gesta closure
references it, and the suite links and passes 2/2 without it.
Precedent set: first `source`-a-generated-snippet idiom in the
repo. Config gained `gesta/fontes` in inclusa (the Phase A gap —
tabularium.h now resolves).

**-plagulae derivation (EXPERIMENTAL, opt-in)**:
`./officina/interpretare.sh -derivare <functio> [args]` — sedes
from build/nexus.tsv, include/X.h → lib/X.c by convention
(irregular sedes → loud refusal, give -plagulae by hand), closure
via --enumerare, filter patterns as `<basis>.c` (substring-exact
against TU titles). Smoke: via_est_absoluta derived exactly
`chorda.c,chorda_aedificator.c,piscina.c,via.c` — 4 modules
instead of the whole lib/ world. Behavior verified IDENTICAL to a
hand-passed -plagulae run (including the pre-existing marshaling
limitation where chorda-by-value args print no result — that is
interpretare's own affair, not the seat's). Opt-in rather than
automatic because silently narrowing the default whole-world
lowering is a semantics change that deserves its own blessing;
also function-pointer escapes (callbacks registered in TUs outside
the static closure) are the known theoretical hole of
closure-as-execution-world.

**Where the arc lands**: all four phases sealed in one day.
bin/aedilis derives, manifests, emits, executes, and self-checks
against clang; the corpus gate certifies 119/119; two consumers
retire folklore (gesta list, bench filter); aedilis.stml holds
eight irregular rules + link truths that previously lived only in
compile_tests' link-everything. Deferred by name: suite adoption
(Fran's call, with track record now in hand), amalgamare list
regeneration (01KY0XRBEX rides the same --enumerare mode),
fabrica absorption, est_angulata upgrade (01KY118F).

## 2026-07-20 — Phase B (emitters + oracle + THE GATE) — sealed

**PORTA: 119/119 OMNES BONAE (149s).** Every root probatio:
derivation → emitted script → build → run (where runnable) →
differentia consensus vs clang -MM. Three gate rounds: 65 → 112 →
119; each round's failures became config truth or engine law.

**Shipped**: aedilis_scriptum_scribere (both postures — cache-
reusing with PRECISE staleness from the manifest's capita list,
not find-everything; solitarius hermetic, own object dir, always
recompiles; both executed live), --differentia / --solitarius /
--currere / --scribere flags, tools/aedilis_porta.sh (the corpus
gate), seven irregularia + CoreFoundation regula in aedilis.stml,
annotations in lib/tempus.c + both speculum probationes.

**Differentia had to learn the link/TU distinction**: clang -MM
sees ONE translation unit; aedilis computes the LINK closure
(tls.h arrives via walking http.c). The honest comparison is
capita vs the UNION of -MM over every closure source — extraction
+ resolution validated per TU, object discovery validated by the
gate's real links. Also: -MM prints paths AS WRITTEN
("lib/../vendor/sqlite3.h" from scrinium's ".." include) —
normalize through via_normalizare before comparing; that one fix
closed ~10 differentia failures from both directions.

**What the gate taught (engine law)**:
1. WALK-CAP 1MB: sources over 1MB are data-as-source (biblia_dr
   6.1MB — silva OOM-killed at exit 137) — recorded, compiled,
   linked, never parsed. Speculum's own LIMES precedent.
2. TEXTUAL .c INCLUDES: #include "persona_gen.c" (nuntium
   generated-code idiom) pours symbols into the includer — the
   convention-probed object for the same file must be purged
   post-fixpoint or the link sees duplicate symbols.
3. SELF-ANNOTATION HAZARD: include/aedilis.h's own enum comment
   /* aedilis: obiectum */ was a perfectly anchored annotation
   with an empty argument — the engine documented itself into a
   refusal. Never write the anchor in prose comments.

**What the gate taught (config truth — the fabrica absorption
beginning in practice)**: irregular header→object rules for
fenestra.h (+fenestra_textus.c — declared-in-header, defined in
companion), biblia.h→biblia_dr.c, entitas_repositorium.h→_impl.c,
nuntium_schema.h→[schema, generare], arbor2_glr.h→[glr, tabula],
persistentia.h→[memoria, nuntium], calendarium_liturgicum.h→
[liturgicum, sanctorale], lapifex_c89.h→grammatica. Plus link
truth: tls_macos needs CoreFoundation (CFRelease) beyond Security.
And lib/tempus.c's platform-seam externs ("implementatae in
fenestra_macos.m" — its own comment confessed) got the second
real annotation in the tree.

**Emitted script shape**: repo-root guard, VEXILLA/INCLUSA from
config, shared flattened object cache (lib__tcp_posix.o) for the
cache-reusing posture, per-target dir for solitarius, vendor
compiled with its own flags and source-newer-only staleness,
absent-source loud guard (generata nondum?), link-flag dedup,
binary at build/aedilis/<basis>/<basis> (never clobbers suite
artifacts), provenance header (generatum/commissum/regeneratio
line). Annotated vexilla apply to the LINK line only (v1 —
compile-affecting annotations would need a distinct verb).

**Exit-code law violated twice more** (porta piped through tail →
exit 0 lie); porta_relatio.txt + explicit `echo $?` is the shape.
Porta debris (porta_*.err/.diff) lives in build/aedilis/.

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

## 2026-07-21 — SOURCE_FILES middle path (dry run for suite adoption)

compile_tests.sh's 139-line hand-maintained SOURCE_FILES/
OBJC_SOURCES world replaced by a 5-line source stanza; the lists
now come from tools/compile_tests_fontes_generare.sh → committed
compile_tests_fontes_generata.sh (128 C + 4 ObjC). Structure
UNCHANGED — still necte-omnia, still one object set — this was
the dry run for adoption, not adoption.

**The audit verdict**: union of aedilis closures over ALL 122
linked principals (119 probationes + generare + capsula_generare +
nuntium_schema_generare) == the hand list EXACTLY. Zero fat, zero
gaps. vendor/sqlite3.c = lane difference (vendores manifest
section; --enumerare prints obiecta only), build/ speculum object
= per-target wiring, correctly excluded. Contrast gesta (utf8 was
dead weight): the root list had been kept honest by discipline —
now it's honest by derivation. Tier-3 (per-target adoption +
fabrica absorption) can be decided with this evidence in hand.

**Generator hardening beyond the generic fontes_generare.sh**:
(1) per-derivation failure checked OUTSIDE any pipe — the generic
one's `|| exit 1` inside a piped-$() subshell can truncate the
union silently (empty-check catches total failure, not partial);
(2) flat-basename collision guard — the suite compiles every
object to build/<basis>.o, two sources sharing a basename would
silently clobber; refuse loud with the colliding names.

Proof: full suite green 103/103 + 14 GUI apps, 133.8s.

## 2026-07-21 — AMALGAMA ARC INTENTIO (phases 0/A/B/C, then D)

Architecture (settled by the two-agent research, notes on
01KY0XRBEX): aedilis owns WHAT + ORDER (closure + derived include-
graph ordering); silva_amalgama stays the ONLY text engine (rename,
CADENDA, static injection, include lifting, gates). Product joining
them = derived manifest parts.

Decisions (Fran, 2026-07-21):
- D1: generic bundle = .h/.c PAIR (house shape; stb single-file
  rejected — fights the flag regime, no precedent).
- D2: feeding = GENERATED C SNIPPET the hand manifest includes
  (mechanism unchanged, compiler keeps enforcing; STML-loading
  rejected — new parser dep in a verified tool for no needed
  capability).
- D3: arc = 0/A/B/C (migration of the three amalgams); Phase D
  (generic bundler, Fran's original use case) = own pull after.
- D4: promotion riders IN-ARC: STML query CLI (jq-for-STML;
  replaces the ×3 hand-rolled section-aware awk) + renamer
  extraction as instrument. API-surface extractor = own named pull
  (4 consumers). All five candidates pinned on 01KY0XRBEX.

Phases:
- 0 SPIKE: (1) WHY does body order matter (S42 compile-proven in
  sim ④, mechanism unrecorded — permute CORPORA in a scratch
  manifest, read the breakage); (2) does topo over the include
  graph reproduce silva's hand CAPITA order (edges via legati
  inclusiones for the spike); (3) does the mechanism accept a
  degenerate manifest (empty rename/cadenda, no hand caput) — the
  Phase-D precondition.
- A ENGINE: fructus records include edges per caput/obiectum (raw
  edges, not baked order); manifest schema children; topo helper +
  loud cycle refusal; fixture pins; porta + speculum-consumer check.
- B EMITTER+AUDITOR: bin/aedilis --amalgama <scopus...> → auditor
  report vs hand manifest (derived-vs-hand buckets) FIRST, then
  committed generated snippet (file arrays in order).
- C MIGRATION: three manifests consume snippets (hand policy parts
  untouched); driver hygiene equalized (01KY2P5X: censor→tessera,
  newest_header→silva/officina); ALL amalgam gates re-run. Order:
  tessera → officina → silva (smallest first, deliverable last).

## 2026-07-21 — AMALGAMA Phase 0 RELATIO (spike: all three answered)

**Q1 — why does body order matter? IT DOESN'T.** Permutation
experiment (scratchpad/permutatio/): full REVERSAL of both corpora
lists → amalgam compiles CLEAN under the entire flag set. Full
reversal of both capita lists → 3 errors, all type-before-use
(FluxusFunctionis used by fluxus_datorum.h before fluxus.h defines
it). S42's "two topologies" folklore resolves to: HEADERS need a
topo sort; BODIES need only determinism. Phase A = ONE sort.
Caveat: body-freedom proven on silva's amalgam; per-project gates
re-verify at migration. Note: full header reversal produced only
3 errors because the hand-written silva.h owns most types and
precedes everything — for generic bundles (no hand header, Phase
D) the topo sort carries the full load.

**Q2 — does derivation reproduce the hand order? YES, as a valid
linearization.** The fontes include graph is fully populated
(headers declare their own edges — including exactly the edge
whose violation Q1 watched fail) and ACYCLIC (tsort clean). Hand
CAPITA order = valid topological linearization (0 violations).
Auditor check = "hand order valid against derived edges", NOT
order byte-equality; migration preserves the validated hand order
as tie-break so the byte-identical regeneration gate holds
trivially; new amalgams get pure derived order.

**Q3 — degenerate manifest accepted? NO (by inspection).**
via_capitis is read UNCONDITIONALLY (sprintf %s with the field, no
NIHIL guard = UB on NIHIL; silva_amalgama.c ~846). Latina inline
also unconditional — but that is CORRECT for house-tier bundles.
Phase D mechanism delta = one small allowance (via_capitis NIHIL →
skip verbatim-caput section), protected by the byte-identical
silva.c regen porta.

Shell gotcha: the session shell is zsh — unquoted $VAR does NOT
word-split (clang got 8 object paths as one arg); ${=VAR} forces
it. Scripts themselves are bash and unaffected.

## 2026-07-21 — AMALGAMA Phase A RELATIO (fructus edges + topo)

AedilisCaput gained `inclusa` (lazy Xar of resolved include vias;
NIHIL = never walked / directive-free). Recording: visa_capitum now
stores the stable AedilisCaput* (was NIHIL) so the walk finds the
including caput for free; edges recorded BEFORE the dedup/vendor
skips (an edge to an already-seen header must not be devoured —
pinned). Capita-only per the Phase 0 result (bodies order-free).

aedilis_capita_ordinare = batch Kahn: each round emits every ready
caput in discovery order (deterministic tie-break); dependency
outside the capita set counts as satisfied; a progressless round
refuses loudly with ALL cycle members named. Manifest: capita carry
<inclusio via="..."/> children.

Test net: fixture edge beta→gamma (chosen so every existing count
stays untouched; also covers the recorded-despite-dedup case since
gamma is already seen via lib/alpha.c when beta.h walks); 3-file
cycle scope (cyclus_a ⇄ cyclus_b); pins: exact topo order
[alpha delta librum gamma epsilon beta], alpha inclusa NIHIL,
cycle refusal naming both members, manifest inclusio round-trip.
First-run green.

Gates: probatio green; PORTA 119/119 (156s) on the new schema;
speculum consumer green (generator awk unaffected by child
elements, both probationes pass). Engine-parity proof: old-vs-new
binary emit BYTE-IDENTICAL closure parts for the wearer (62
entries) — the bundle's 78-vs-80 plagulae delta vs the memory note
predates Phase A (point-in-time number at 9bf8c7b), not a schema
effect.

## 2026-07-21 — AMALGAMA Phase B RELATIO (emitter flags + auditor)

CLI: --partes (fructus TSV: O/C/S/V), --aristae (edge dump),
--ordo (topo order, cycle = refusal exit 1). All return before
manifest emission (no side effects, --enumerare precedent). The
three amalgamator manifests gained --enumerare (CV/OV/CP/OP TSV
from the hand arrays — machine-readable truth from the artifact
itself; survives Phase C as a live loop-closure check). All three
amalgams regenerate BYTE-IDENTICAL with the edit (git clean).

tools/amalgama_auditor.sh = the Phase B product (eyes-tool,
differentia-mode, exit 0 unless operational): per project runs
amalgamare.sh (own gates), dumps hand lists, derives the union of
--partes over the OV+OP roots, buckets CONSENSUS/DERIVATA-SOLA/
MANUS-SOLA, validates the hand capita ORDER against derived edges.

**AUDIT VERDICT (first full run)**:
- ZERO MANUS-SOLA anywhere — aedilis sees everything all three
  manifests carry. Hand orders = valid linearizations ×3.
- silva: EXACT modulo one lane (latina.h = the mechanism's special
  inline, not a file-list entry). Corpora 23/23.
- tessera: derived-only lib/chorda.c + chorda.h = the DELIBERATE
  type-only exclusion (TesseraChorda owned by tessera.h; body dead
  weight dropped by hand). File-level dead-code policy the snippet
  consumer must be able to express.
- officina: derived-only silva/amalgama/silva.{h,c} = the
  hospes-duplex structural dependency (handled via PROOEMIUM, not
  file lists) + officina_indicium.h = public-header ownership lane
  (officina.h covers the indicium API).

**Phase C hinge**: findings shape the snippet format — derived
lists need per-file HAND OVERRIDES (exclude-body: tessera chorda;
external-dependency: officina silva.h; the latina lane stays
mechanism-special). Migration = snippet + small exclusion policy
in the manifest, not blind list replacement.

Auditor first-run crop: stale bin/aedilis silently lacked the new
flags ([ -x ] existence check ≠ freshness) — auditor now ALWAYS
runs aedilis_struere.sh. The excubitor class strikes the very tool
built to audit staleness; noted with due irony.

## 2026-07-21 — AMALGAMA Phase C RELATIO (migration of the three)

**All three amalgam manifests now consume generated lists — every
amalgam BYTE-IDENTICAL through migration** (git diff empty on all
three amalgama/ dirs + latina datum; the strongest possible
migration proof, enabled by preference-seeded stable topo).

Shape shipped: tools/amalgama_fontes_generare.sh emits
fontes_generata.h (four AmalgamaPlagula arrays, uniform names
CAPITA/CORPORA_VENDICATA/PROPRIA) next to each manifest;
amalgamator.c replaces ~60 hand lines with one #include; policy
lives in fontes_politica.sh per project (radices glob + exclusa
with named causes + servanda/excludenda macro names per basis —
the macros themselves stay in amalgamator.c, so the compiler still
enforces the pairing). Order: capita = stable Kahn (preference =
prior snippet, else --enumerare of the current binary = migration
seed); corpora = preference order (order-free, Phase 0); emitted
order re-validated against edges before write (defense).

Policy classes the audit predicted, now expressed: tessera
chorda.{h,c} = type-only exclusion; officina silva/amalgama/{h,c}
= external architectural dependency + indicium.h = public-header
ownership; silva sceleti/imparilis = fixture tables never
amalgamated; latina.h = mechanism lane (generator-global).

Hygiene (desideratum 01KY2P5X): newest_header ABI guard added to
silva + officina drivers (tessera's precedent); censor gate added
to tessera (officina's shape). All three drivers VERIFICATUM.

Post-migration auditor: MANUS-SOLA empty x3, orders valid x3,
DERIVATA-SOLA = exactly the seven documented lane entries. The
loop is CLOSED: --enumerare now prints the generated lists, so
every auditor run compares the consumed truth against fresh
derivation — drift clamat.

Steady state: add an include/new fontes file → regenerate snippet
→ amalgamare gates verify. The hand-maintained surface is now
pure policy with named causes.
