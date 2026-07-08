# Officina simulatio 1 — interview (M0 semantica)

Date: 2026-07-08. Follow-up to officina-simulatio-1.md (the six open
questions + five the interview surfaced). All eleven answers landed on
the recommendation. Raw Q&A:

**Q1. Praebenda systematis: scope and form?**
A: FULL C89 libc surface — all 15 ISO headers' worth, ~145 functions +
core types, hand-written once, versioned lexicon fixture (latina.h
precedent). It IS officina's platform definition file. (Over:
audit-scoped ~105 symbols; generated-from-audit.)

**Q2. M0a / M0b split?**
A: Split as proposed. M0a = declarations + scopes + types + layout +
oracle + index + ambiguity closure (demo: ambigui 15k -> ~0). M0b =
expression typing + explicit conversions. (Over: monolith; finer split
with unverified layout.)

**Q3. Expression-type storage?**
A: Parallel tables at analysis time (nodus -> {typus, conversio}),
oracle-responsa precedent; demissio reads, quaestio/saltuarius can
query. (Over: demissio-time recompute; lazy memoized.)

**Q4. Oracle coverage for block-scope types?**
A: Coverage argument + named park — file-scope ~415 nameable types
directly verified; block-scope runs identical code path; park recorded
with replicate-and-verify as the path back. (Over: synthesized-tag
replication; silent skip.)

**Q5. Type-error policy?**
A: Poison type + diagnostics table — TYPUS_ERROR absorbs without
cascading, analysis always completes, never crash (tree-sitter bar
extended to semantics). (Over: first-error stop; hard assert.)

**Q6. Configuration scope?**
A: Taken arms only — semantica is per-configuration; multi-config
typing NAMED into the config-query milestone (re-analysis under an
atom assignment). (Over: all-arms now = pulling config-query into M0.)

**Q7. Cheap parks (K&R; lifetime)?**
A: Both approved. K&R typed as unprototyped, corpus cannot contain it
(-Wstrict-prototypes -Werror); semantica lifetime nests inside
parsura's piscina, REPL invalidation = M4 + incrementalitas journal.

**Q8. Index: first consumer, persistence?**
A: In-memory API, saltuarius type-jump first (v0.1 candidate),
demissio second; persistence parked until a cold-start consumer
exists. (Over: persisted artifact day one; officina-only.)

**Q9. Type handle shape? (Eskil decision in miniature)**
A: Single pointer (TypusC89*) everywhere; qualified variants interned
as wrapper nodes like any derived type; pointer equality = identity
for derived types; tag types nominal per C89. (Over: {typus, vexilla}
pair handles; structural comparison.)

**Q10. Module family / result object names?**
A: semantica / SilvaSemantica. Modules: silva_c89_typus,
silva_c89_scopus, silva_c89_forma, umbrella silva_c89_semantica with
silva_c89_semantica_analysare(). (Over: anima — collides with the
existing "anima semantica" section meaning the oracle dance;
intellectus.)

**Q11. Platform file / oracle instrument names?**
A: **systema** (silva/fontes/systema_c89.h as committed text fixture,
compiled in via the latina-datum mechanism) + **haruspex** (the
layout-oracle emitter instrument: reads the entrails of every form
and pronounces against clang). (Over: probator_formae;
bibliotheca_normae.)
