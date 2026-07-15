# GESTA K2 — INTERROGATIO (raw Q&A)

2026-07-15. Interview for K2 (relationes propriae: typed links + attribute
schemas + salus + opera). Compressed ritual per Q10. Oracle sections read
directly before questioning: smaragda.ts Relationships (3673–4187), Health
(4188–4513), Tasks (4514–4654), Genus types (777–894), defaultReducer member
semantics (759–770), genusReducer/getGenusDef (1802–1863),
_validateAttributeType (1790–1798).

## Round 1 — K1-doctrine collisions

**Q1. Nexus fate.** K1 shipped a flat nexus fold (res→verbum→res). K2 brings
typed relationship genera with roles/cardinality. What happens to nexus?
(Store holds exactly ONE nexus event — migration cost at lifetime minimum.)

**A: SUBSUME.** Nexus becomes a seeded built-in link genus (two partes a/b,
cardinality unicus/unicus, verbum attribute). One machinery forever; gerere
nexus stays as sugar over it. Small replay shim (tombstone) for the one
existing legacy event. Eskil's law: one link concept, decided early.

**Q2. Genus evolution.** K1 sealed emendatio-generis as WHOLE-REPLACE; TS
evolveGenus is additive-only merge. Which semantics with attribute schemas
arriving?

**A: WHOLE-REPLACE STAYS.** emendatio-generis remains the only genus-change
event. Additive evolve = client courtesy (read def, merge, emit emendatio),
not kernel semantics. One event meaning; salus makes breaking changes
visible.

**Q3. Validation posture.** TS validates relationships hard (throws on
missing role, wrong genus, cardinality). K1 doctrine is record-don't-block.

**A: UNIFORM RECORD-DON'T-BLOCK.** Cardinality/genus/role violations become
custodia notae and surface via salus; the event lands, the member index
reflects reality. The log never lies about what was attempted.

**Q4. Salus scope.** TS bundles health with an Error sentinel genus
(open→acknowledged) — but TS has no custodia notae; we do.

**A: SALUS ONLY.** Pure evaluation (missing required attrs, bad types,
invalid status, cardinality) + listUnhealthy-style query + census surfacing.
NO Error genus — custodia notae already are our error stream. Defer an
erratum genus until a tenant needs acknowledged-error lifecycle.

## Round 2 — scope edges + API surface

**Q5. Opera scope.** Under genera-as-events, tasks might be nearly free (an
opus genus is just a definitio-generis event).

**A: GOLDENS-ONLY PROOF.** A probatio golden defines an opus genus with
attribute schema and walks claim/complete through existing
creatio/mutatio/status events. No tenant seed change, no new API. Tabularium
gets an opus genus later, on pull (likely K3 actions).

**Q6. Attribute type vocabulary.** TS: text|number|boolean|filetree. Note
tags is already an ARRAY attribute in daily use.

**A: FOUR TYPES: textus, numerus, veritas, TABULATUM** (JSON array). Without
tabulatum, tags could never be schema'd. Skip filetree (palace-domain,
K4–K5).

**Q7. MCP surface.** K1 layering: kernel maximalist, tenant vocabulary
small.

**A: KERNEL-COMPLETE, MCP QUIET.** Full kernel API ships; tabularii tools
stay as-is — gerere nexus keeps working as sugar over the subsumed
machinery; salus surfaces passively in res/census output where cheap.
Dedicated tools arrive on tenant pull.

**Q8. Naming.** Does the kernel need a NEW word for typed relationships?

**A: NEXUS IS THE WORD.** Genus datum gets species:"nexus"; roles are
"partes"; the seeded simple two-ended genus is just "nexus". No "relatio"
anywhere. Supporting slate: partes/cardinalitas, membrum-additum/-remotum,
salus, opus.

## Round 3 — dogfood + process

**Q9. Seed v2.** Schemas proven in goldens only, or emend the five LIVE
tabularium genera so salus checks the real store?

**A: EMEND LIVE GENERA.** Idempotent emendatio-generis at initialize gives
seeded genera real schemas; salus runs against the actual store from day
one. The tenant is the test bench (same reasoning as tabula.md/doctrine in
K1.1). Violations surfaced in existing data are findings.

**Q10. Ritual weight.** K1 was interview → spec v1 → 3 explorers → spec-v2.

**A: COMPRESSED: SPEC → BUILD.** One self-contained spec (K1-spec-v2
format), Fran reviews/commits, then build in chunks. Remaining oracle
details verified by direct reading during spec-writing (done: reducer member
semantics, genusReducer, _validateAttributeType — see header).

## Decisions carried into spec

1. Nexus subsumption + tombstone shim (D2 in spec ledger)
2. Whole-replace stays (D6)
3. Record-don't-block extends to membra (D3)
4. Salus only, no Error genus (D4)
5. Opera = vocabulary, proven in goldens (D10)
6. Types: textus/numerus/veritas/tabulatum
7. MCP quiet; passive salus surfacing
8. Naming: nexus/partes/cardinalitas/membrum-additum/-remotum/salus/opus
9. Seed v2 emends live genera (dogfood)
10. Compressed ritual, spec is self-contained
