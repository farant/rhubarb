# Simulatio ⑦ — CONDITIONALIS threading (Phase 7 Chunk B)

*2026-07-03. Mid-phase pretend-implementation against SHIPPED code
(the ⑤/⑥ technique, now 2-for-2 at reducing work). Question: how do
conditional regions (SilvaRegio/SilvaRamus on the expansio) thread
into committed parse trees, with the one-owner byte rule held and
zero byte drift across the migration? Every claim below is verified
against source with file:line references. Scope per the Phase 7
INTENTIO: minimal threading — taken arm parsed, untaken arms as
token payloads; untaken-arm SUB-PARSING stays at the config-query
milestone.*

## Update stream

**T+5** — Reading the target structures. genera-extra already carries
`conditionalis` in BOTH grammars (generator REQUIRES it, spec §12.2)
— but its shape is `rami:lista-nodus` ONLY (sceletum.stml:110). No
ramus genus exists, no slot for the closing #endif line. The
structural-genera requirement means the genus ENUM constant and
registrum entry are already baked and the engine already tolerates
the genus; only the shape needs extending. Good start: the node
vocabulary is half-built.

**T+10** — Where do the bytes live today? Three owners:
(a) taken-arm tokens flow into reliqua and thence the tree (regions
are INVISIBLE in the token stream — nothing marks their boundaries);
(b) untaken arms sit as lexemata_cruda on rami
(silva_expandere.h:105); (c) ALL directive lines — #if/#elif/#else/
#endif included — go through _directivam_capere into the flat
directivae_out stream (silva_expandere.c:626 arm lines, :801 ENDIF,
:1231/:1234 guard-file lines). scribere_fontem reinserts (b) + (c)
by offset (silva_scribere.c:625-644). First hard fact: **the taken
arm records NEITHER its body extent nor which reliqua tokens are
its** — and the #endif line is captured but NOT stored on the
region. Threading needs both. → C1.

**T+15** — Trying to attach at the GLR/grammar layer (region nodes
as parse-time constructs, spec §8.1.4's long vision): the sub-parse
contract (§10.2.2, GSS-state sub-parses) is config-query-milestone
machinery; nothing in today's engine sees region boundaries. Dead
end for Chunk B — the expanded stream simply has no markers. Pivot:
thread AFTER parsing, from the region tree + offsets.

**T+20** — Found the natural layer. The driver builds the radix
lista itself (slicing loop, silva_parsare.c) and THEN calls
silva_committere (:343 area). Between those two points the tree is
driver-owned, pre-commit, pre-pater — a threading pass there
violates nothing (S26 purity is about GLR constructions; the driver
already hand-builds ERROR nodes at this layer, :50-79 pattern).
And the commissio walk is GENERIC over loci
(silva_commissio.c:130-132 commits every locus recursively) — a
conditionalis node inserted pre-commit gets pater fixup + list
normalization FREE. → C4 (one wrinkle: _lexemata_parsare_interna
doesn't see the expansio; the fistula must pass it down — a param,
same shape as the contextus threading just done in Chunk A).

**T+25** — Detection mechanics. Region boundaries are layer-0 byte
offsets (ramus->directiva tokens carry byte_offset; body bounds =
[directive-line end, next structural directive start) — KNOWN inside
_regionem_processare as i_corpus/i_scan but NOT STORED). Statements
are built from EXPANDED tokens — but every token's layer-0 position
is recoverable: FONS tokens directly, EXPANSIO tokens via the
invocation radix (the Phase 5 boundary machinery,
silva_scribere.c:253). So: per radix element, compute a layer-0
extent [first,last] by walking loci in layout order; per region,
compare against the taken arm's stored body range. Alignment =
every element overlapping the region falls FULLY inside the arm
body. → C1 gains: store corpus bounds on rami (2 s32, set during
region processing); C3 names the extent walk.

**T+30** — The straddle taxonomy (the INTENTIO's named hazard),
resolved by the detection shape:
- **whole statements inside arm** → thread (the friendly case);
- **region inside a statement** (`int x = /#if/.../#endif/ ...;`) →
  the statement's extent straddles the region start → detection
  fails → DEGRADE: region stays reinserenda-owned, tree untouched,
  roundtrip unaffected (today's behavior IS the fallback);
- **region straddling statement boundaries** → same degrade;
- **statement with multi-fons layer-0 tokens** (include splice
  mid-statement) → extent ill-defined across fontes → degrade;
- **tail-ERROR from cancellation** (Chunk A) spans everything →
  overlapping regions degrade automatically.
Degrade-not-fail is the posture everywhere; count it. → C8.

**T+35** — Emission through the generic walk, traced: a conditionalis
element in the radix lista hits _valorem_scribere → _nodum_scribere →
loci in layout order. Proposed shapes: conditionalis {rami:
lista-nodus, finis:lista-token}; ramus-sumptus {directiva:
lista-token, contentum:lista-nodus}; ramus-omissus {directiva:
lista-token, cruda:lista-token}. Layout order == byte order for
every genus (directive line precedes content; #endif follows last
arm) — the generic emitter needs ZERO new cases. ERROR already
proves lista-token loci emit verbatim through the walk. The
est_sumptum flag cannot live on SilvaNodus (genus + loci only, no
flag fields) → the genus SPLIT carries it, and the query surface
wins selectors for free (`ramus-omissus` as a tag). → C5.

**T+40** — The one-owner migration problem: threaded regions' lines
and cruda must LEAVE reinserenda or every roundtrip double-emits.
Two designs:
- **α (filter)**: mark threaded regions est_texta; scribere skips
  their cruda in _regiones_colligere and filters the flat directivae
  stream by offset-ranges (or first-token pointer identity — capture
  and ramus->directiva share token POINTERS, both _lamina_capere
  copies from the same stream).
- **β (ownership inversion)**: region STRUCTURAL lines stop entering
  directivae_out at all (remove captures at silva_expandere.c:626 +
  :801; add regio->directiva_finis storage); scribere collects lines
  from the REGION TREE for unthreaded regions (extending
  _regiones_colligere, which already walks it) and threaded regions
  emit theirs from the tree. The partition becomes STRUCTURAL:
  directivae = non-region directives only (define/undef/include/
  pragma/guard-file lines — guard captures :1231/:1234 STAY, guard
  files have no region). No matching, no filtering, no double-emit
  possible by construction.
β is the cleaner one-owner story and _regiones_colligere already
exists; cost = probatio_silva_regiones assertions on captured-line
counts need deliberate revision (they assert internals that change
meaning). → C2, RECOMMEND β.

**T+45** — Nesting, traced through the region tree: filiae hang off
taken arms (silva_expandere.c:544-563). Thread BOTTOM-UP (inner
regions first): an inner conditionalis replaces its statements in
the radix... no — inner statements aren't IN the radix as separate
elements; they're just elements like any other (slicing is flat).
Correction: bottom-up means process the region TREE depth-first;
inner region groups elements into an inner conditionalis node,
which then has an extent (its directive tokens' offsets) and is
itself groupable by the outer region. The flat radix + extent
comparison handles nesting with NO special case — the recursion is
in the region tree walk, not the grouping. Empty taken arm → empty
contentum; region containing only consumed directives (#defines) →
zero grouped elements, conditionalis still inserted at its
offset-ordered position (elements are offset-ordered because
segments are).

**T+50** — Fons-uniformity, verified: the emitter skips
foreign-fons tokens at BOTH the FONS arm (silva_scribere.c:228-243)
and the boundary arm (:260-264). So a conditionalis node whose
tokens belong to an included fons emits NOTHING when reconstructing
the includer (its bytes ride the #include line reinsertion) and
emits fully when reconstructing its own fons — threading is
fons-agnostic, included-fons regions thread with ZERO special
casing. The C7-fix regions (predefined-custos guard interiors,
untaken arm) thread as ramus-omissus like anything else. → C7.

**T+55** — Edge sweep against the robustness bar:
- est_imperfecta (EOF before #endif): finis slot empty — emits
  nothing — byte-exact (there IS no #endif). Threads fine.
- est_ultra_modum (Chunk A cap): ALL arms cruda → all ramus-omissus,
  no contentum. Threads fine (and its bytes leave reinserenda —
  the cap and the threading compose).
- est_numquam (#if 0): ordinary untaken arm. A distinct
  ramus-numquam genus is tempting for queries but the journal/rami
  table already carries est_numquam — DEFER, note in register.
- Orphan #elif/#else/#endif (no region): pass through reliqua as
  opaque tokens today (Phase 2 decision 7) — untouched by threading.
- Fuzz shapes: detection is pure offset arithmetic + degrade —
  no crash surface; the 400-specimen oracle guards it.

**T+60** — Existing-test impact scan: probatio_silva_regiones
asserts captured-directive-line COUNTS (C7 test: "2 directive lines
captured") — β changes those counts (region lines leave the
stream) → revise deliberately, log in phase-log. probatio_silva_
scribere region fixtures assert ROUNDTRIP mostly — stay green if
threading is byte-neutral; some may now exercise threading
implicitly (good). Corpus 78 + syntaxis 125 + fuzz 400 + cursor
closures ×2 = the real migration net: zero byte drift required.
Counters: parsura gains regiones_textae / regiones_omissae (product,
not debug — the degradation rate on solarium in Chunk C is a
DESIGN INPUT for the config-query milestone).

**T+65** — Effort shape: expander +~15 lines (directiva_finis,
corpus bounds on rami, two capture-call removals under β); grammars
+2 genera each ×2 files + regenerate (generare.sh); driver threading
pass ~150-200 lines (extent walk + region-tree walk + grouping +
lista rebuild + form lookups by name); scribere ~30 lines
(_regiones_colligere extension); probatio_silva_conditionalis
~150-200 lines; probatio_silva_regiones revisions. No engine
(GLR/commissio/nodus) changes AT ALL — the generic walks absorb
everything. That is the headline: the maximalist data model keeps
paying.

**T+70** — What this does NOT foreclose (VISIO check): untaken-arm
sub-parsing later replaces ramus-omissus cruda with contentum at
full fidelity (same node, richer slot — the pinned transforms
requirement governs); config-scoped queries get conditionalis nodes
with journal conditio_id linkage (rami order matches exp->rami
indices — worth STORING conditio_id per ramus node? An index-species
locus carries it — ADD conditio_id:index to both ramus genera, one
locus, links tree↔journal↔regions permanently. Cheap now, painful
to retrofit. Added to the shapes.) Layer-N views unaffected.
Formatter unaffected (trivia rides tokens).

**T+75** — Register closed. Verdict below. The INTENTIO's open
question ("slicing layer, commit walk, or post-commit pass") answers
itself once traced: NONE of the three as originally framed — it's a
**pre-commit driver pass over the flat radix + region tree**, with
detection by layer-0 extents and degrade-not-fail posture.

## Complexity register (speculum schema)

**C1 — Region extents are not recorded where threading needs them.**
- discovered-while: locating region boundaries in stream coordinates
  (T+10/T+25).
- consists-in: taken rami store no body range (i_corpus/i_scan are
  locals, silva_expandere.c:712-714); the #endif line is captured
  into directivae (:801) but not stored on SilvaRegio.
- consequences: no threading detection is possible from today's
  structs alone.
- handled-by: SilvaRamus gains corpus_initium/corpus_finis (s32 byte
  offsets, set for ALL arms); SilvaRegio gains directiva_finis
  (Xar*, _lamina_capere at the ENDIF exit). ~10 expander lines.

**C2 — Region directive lines live in the flat capture stream.**
- discovered-while: designing the one-owner migration (T+40).
- consists-in: _directivam_capere feeds ALL lines into one
  directivae_out; threaded regions would double-emit unless filtered
  (α) or ownership is inverted (β: region lines never enter the
  stream; scribere collects unthreaded regions' lines from the
  region tree, which _regiones_colligere already walks).
- consequences: α = matching logic forever; β = structural partition,
  no filtering, but probatio_silva_regiones internals-assertions
  need deliberate revision.
- handled-by: RECOMMEND β. Decision for Fran.

**C3 — Statement extents must be computed from expanded tokens.**
- discovered-while: alignment detection design (T+25).
- consists-in: radix elements hold EXPANSIO-origo tokens; layer-0
  position = invocation radix offset (machinery exists,
  silva_scribere.c:253 _radix_probata); extent = first/last such
  offset over loci in layout order.
- consequences: an extent walk per radix element (~40 lines); PASTA/
  CHORDA/API-origin tokens have no radix → treat as extent-opaque →
  their statements degrade any overlapping region (consistent with
  the named Phase 5 deferrals).
- handled-by: _extentum_computare in the driver; multi-fons
  statements degrade (C8 posture).

**C4 — The threading layer is the driver, pre-commit.**
- discovered-while: T+15/T+20 (grammar-layer attach is config-query
  machinery; post-commit violates immutability).
- consists-in: pass runs between slicing and silva_committere;
  radix is driver-owned there (ERROR-node precedent); commissio's
  generic loci walk (silva_commissio.c:130) gives pater/normalization
  free.
- consequences: _lexemata_parsare_interna needs the expansio (param
  from _fistula_interna; NIHIL for the token-stream entry = no
  threading, correct — no regions exist there).
- handled-by: one internal param + the pass (~150-200 lines).

**C5 — Node flags don't exist; genus carries arm-kind.**
- discovered-while: representing est_sumptum on ramus nodes (T+35).
- consists-in: SilvaNodus = genus + loci only.
- consequences: one ramus genus + flag is unrepresentable without an
  index-locus hack that queries can't select on.
- handled-by: genus split ramus-sumptus/ramus-omissus (selector tags
  free); est_numquam stays journal-side (deferred, noted); BOTH gain
  conditio_id:index locus linking tree↔journal↔regions (T+70).

**C6 — Nesting threads via the region tree, not the radix.**
- discovered-while: nested-region trace (T+45).
- consists-in: depth-first region-tree walk; inner conditionalis
  becomes an ordinary extent-bearing element for the outer grouping;
  flat radix + offsets need no nesting special case.
- consequences: none beyond walk order.
- handled-by: bottom-up walk in the pass.

**C7 — Threading is fons-uniform for free.**
- discovered-while: included-fons region trace (T+50).
- consists-in: the emitter's fons filters (silva_scribere.c:228-243,
  :260-264) already skip foreign tokens per reconstruction target.
- consequences: included-fons regions thread identically; no scope
  carve-out needed ("principal fons only" would be MORE code).
- handled-by: nothing — verified property.

**C8 — Degrade-not-fail is the detection posture.**
- discovered-while: straddle taxonomy (T+30) + fuzz sweep (T+55).
- consists-in: intra-statement regions, straddling regions,
  multi-fons statements, extent-opaque (pasta) statements, tail-ERROR
  overlap → region stays slice-owned (today's proven behavior IS the
  fallback); threading is strictly opt-in per region.
- consequences: partial threading is normal, not exceptional;
  visibility required.
- handled-by: parsura counters regiones_textae/regiones_omissae
  (product); solarium's degradation rate (Chunk C) feeds the
  config-query milestone design.

## Verdict

**Build it.** No engine changes; expander +~15 lines; the pass is
~150-200 driver lines against machinery that already exists
(_radix_probata, _regiones_colligere, ERROR-node construction
pattern, generic commissio/scribere walks). The INTENTIO's hazard
(straddling) dissolves into detection + degrade rather than a
correctness cliff. The migration net is the standing oracle set —
zero byte drift is directly asserted by corpus/fuzz/closure tests.

## Decisions — RESOLVED (Fran, 2026-07-03, all recommendations taken)

1. **β — ownership inversion** for region directive lines: regions
   own their structural lines ALWAYS (capture calls at
   silva_expandere.c:626/:801 removed; directiva_finis stored;
   scribere collects unthreaded regions' lines from the region tree).
   directivae_out = non-region directives only. probatio_silva_
   regiones internals-assertions revised deliberately.
2. **Genus split**: ramus-sumptus {directiva, contentum, conditio_id}
   / ramus-omissus {directiva, cruda, conditio_id}. est_numquam
   stays journal-side (named deferral).
3. **conditio_id:index locus on both ramus genera** — the
   tree↔journal↔regions join key, pinned now.
4. **Counters**: regiones_textae + regiones_omissae on SilvaParsura;
   Chunk C's solarium run measures the real-world degradation rate
   as config-query-milestone design input.

Chunk B scope is FIXED: expander (directiva_finis + corpus bounds +
β capture removal) → grammars (conditionalis finis slot + two ramus
genera ×2 files, regenerate) → driver pass (extent walk + region-tree
walk + grouping + counters) → scribere (_regiones_colligere collects
unthreaded lines, threaded regions skipped) → probatio_silva_
conditionalis + regiones revisions + standing oracles (zero byte
drift).
