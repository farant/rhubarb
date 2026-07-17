# Fluxus-0 interview — raw Q&A (2026-07-17)

Context: after the FLUXUS-0 INTENTIO (silva/phase-log.md) and
simulatio XI (project-specs/silva-simulatio-11.md), Fran was
interviewed on the open design decisions. 11 questions, 3 rounds.
Every recommendation was accepted; recorded verbatim below.

## Round 1 — the simulation's four open questions

**Q1. noreturn stance for codex 63.** If the differentia sweep over
the 820-file corpus finds disagreements from noreturn functions
(clang knows exit/abort never return; our CFG doesn't), what's the
v1 stance?
**A: Measure first, decide after.** Ship the parity tier, let the
sweep COUNT the noreturn disagreement class, then pick the fix with
the number in hand. Count 0 ⇒ no design at all.

**Q2. Fall-through suppression for codex 64.** TOLERA-only, or also
honor the inherited /* fall through */ comment idiom (GCC honors it;
clang never; corpus speaks it exactly once, frozen arbor v1)?
**A: TOLERA-only.** One uniform suppression grammar house-wide. The
single frozen site stands as standing census (codex 61 precedent) or
gets a TOLERA if arbor_syntaxis is ever touched.

**Q3. Duff grammar gap.** The grammar rejects Duff's device (legal
C89 → whole-function ERROR node, zero corpus instances).
**A: Accept + document.** The nota (01KXRF67RF…) already carries the
named change-seat (flat group scan in fluxus would need a recursive
label walk). No grammar work; revisit only if real code needs it.

**Q4. fluxus API surface.** Internal-only v1, or exported through
the amalgam's public silva.h?
**A: Internal v1.** Amalgam includes the module; silva.h exports
only the new codex enum rows. Eskil's law — let the CFG shape season
under internal consumers before freezing a public surface.

## Round 2 — surfaced during interview prep

**Q5. Verdict-parity hole (verified live during the interview):**
clang ERRORS on frange-outside-loop/switch and salta-to-undeclared-
label; examen says ACCIPE with zero findings. The fluxus walk
computes both facts as byproducts. Close it in this arc?
**A: Yes, in chunk 2.** Two VIOLATIO codices (66 FRANGE_EXTRA_
CONTEXTUM covering frange/perge, 67 SALTA_AD_TITULUM_IGNOTUM),
verdict-flipping per the parity contract.

**Q6. Severity gradus for codex 63.** Stock clang warning
(-Wreturn-type, in -Wall); real UB when the caller uses the value.
**A: SUSPECTUM.** Same shelf as REDDE_SINE_VALORE (the other
return-shape judgment); signals "clang would flag this", not house
taste. (64/65 default to DOMESTICUM per the streak — house-blind
oracle flags; not separately asked.)

**Q7. CFG retention.** Do built CFGs stay in sem after tiers run?
**A: Retain all** (Xar* fluxus_functionum, one per function).
Code-is-a-database; same piscina, near-zero marginal cost; the
arena-bytes meter shows the curve.

**Q8. Codex 65 (sententia inattingibilis).** In-arc stretch or
parked?
**A: In-arc stretch.** ~60 lines once attingibilitas is marked;
completes the stage-0 harvest; dropped without ceremony if the arc
runs long.

## Round 3 — names, cadence, process

**Q9. Latin naming as drafted?** Module silva_c89_fluxus; structs
FluxusFunctionis / FluxusBlocus / FluxusMargo; edge genera SEQUENS,
VERUS, FALSUS, CASUS, ORDINARIUS, RUPTURA, PERSECUTIO, SALTUS,
REDDITUS, CADIT, LAPSUS; codices 63 SEMITA_SINE_REDDITU, 64
CASUS_LAPSUS, 65 SENTENTIA_INATTINGIBILIS, 66 FRANGE_EXTRA_
CONTEXTUM, 67 SALTA_AD_TITULUM_IGNOTUM; sem field Xar*
fluxus_functionum.
**A: As drafted.** (Pre-ship renames are free; enum rows are the
only API.)

**Q10. Cadence.** Build chunk 1 this session or compact first?
**A: Compact first.** Interview + spec files written now; chunk 1
starts with full headroom and the spec as re-entry point.

**Q11. Spec process.** Run the usual v2 agent-exploration pass?
**A: Skip v2, sim stands.** Simulatio XI is the codebase-planning
layer; the spec cites it.
