# Silva — Simulatio ⑤ Interview (2026-07-02)

Raw Q&A following simulation ⑤ (SPPF-style metamorphosis + same-wave packing,
silva-simulatio-5.md). Eight questions in two rounds; every recommendation
confirmed. Decisions feed spec-v2 §12.

---

## Round 1 — the simulation's open questions

**Q1 (Fork cost, C4).** With semantics moved out of the drain, every
typedef-shaped statement forks and builds both interpretations even when the
oracle already knows the answer. Bounded (~2× node construction from conflict
to statement end, collapsed at commit). Accept for M1, mensura gates the
pruning hook?

**A: Accept, measure.** Ship always-fork in Chunk B with counters; the
action-filter pruning hook stays a named, evidence-gated mitigation. Drain
stays semantics-free, engine uniform.

**Q2 (Test grammar).** Where does the unequal-depth variant grammar (the only
grammar that exercises metamorphosis) live?

**A: Committed variant.** Second .stml in grammatica/ with committed generated
tables, drift-guarded like sceletum. Uncommitted test infrastructure rots.

**Q3 (Dup arms).** The C2 ordering rule lets genuinely-equal duplicate
derivations survive within one drain until the frontier merge (one token
later). Slight frontier-width growth for closing the retroactivity hazard.

**A: Accept, watch width.** frons_maxima tracks it; frontier merge dedups per
token; revisit only with corpus evidence.

**Q4 (Contract home).** Where is "contents always via ponere; identity
re-bindable by the engine until commit" recorded?

**A: CLAUDE.md + spec.** One line in silva/CLAUDE.md (auto-loading orientation
root) + full statement in spec-v2 §12. Data-model commitments are what VISIO
exists to protect.

## Round 2 — vision-weight questions the simulation surfaced

**Q5 (Collapse policy).** When the commit walk resolves an AMBIGUUS because
the oracle already knew (e.g. `size_t * x;`), what happens to the losing
interpretation? (code-is-a-database tension)

**A: Collapse + journal.** Tree gets the clean winner, no wrapper; the commit
walk records a resolution event (position, discriminating identifier, winner)
in the fructus — queryable like the expander journal. "What would parse
differently if X weren't a typedef?" stays answerable without bloating every
declaration in real C.

**Q6 (Genera-extra contract).** Engine depends on the grammar providing
AMBIGUUS (fabrica callbacks) and ERROR (recovery). Generator requirement?

**A: Generator requires them.** Missing structural genera (ambiguus, error,
conditionalis) = generation error with a clear message. No silent no-packing
fallback (that would be the smoothed-over-quirk failure mode by construction).

**Q7 (Counters).** How public is parse metadata (fusiones, transmutationes,
new-edge events, frons_maxima)?

**A: Fructus fields.** Part of SilvaGLRFructus — per-parse, caller-visible;
mensura reads from there. Full positioned event journal = named post-M1
upgrade if solarium wants to visualize fork activity.

**Q8 (Naming).** Load-bearing words for the new machinery.

**A: transmutatio family.** `_transmutare` (identity re-bind),
`fabrica ambigui` (ambiguum_creare / ambiguo_addere), `index reducendorum`
(per-drain (status, basis) map), `eventa_marginis_novi` ("new-edge events" —
replaces the proper-name "Farshi events" in code; the literature name stays in
docs).

---

## Net effect on the plan

Phase 4 sequence confirmed as: **A½** (length-view lists, correctness floor) →
**Chunk B** (structural equality + frontier merge + same-wave packing +
transmutatio + counters-in-fructus + accessing-symbol validation + variant
grammar) → **Chunk C simplified** (commit walk = pater fixup + list
normalization + oracle resolution with collapse+journal + re-canonicalization
API; resolutor-at-merge deleted from the plan) → **Chunk D** (driver, recovery,
robustness, mensura integration).

Evidence-gated (named, unbuilt): action-filter pruning hook (C4);
identity-handle refactor if eventa_marginis_novi ever fire meaningfully (C5).
