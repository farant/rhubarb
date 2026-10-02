# natura.worklog.md (lib/natura.c - onerator)

## 2026-08-06 — the loader shrank to its irreducible core

Rules V, VII (format half), VIII, XII and XV migrated OUT of this
file to natura/natura.canon (judged inside natura_examen via
lib/canon.c). What remains is exactly what no schema can hold:
cross-file resolution and inheritance — rules 2-4, 6, 7-ordering
(cross-attribute comparison), 9-11, 13, 14, 16, 17.

Post-migration contract, stated in the header comment: the loader
PRESUMES canon-sane input. Unknown elements are silently walked
through; a nameless genus/species is silently skipped (it cannot
be registered); a duplicate name keeps first-wins registration
(graph integrity) with NO diagnostic — the canon shouts, the
loader loads. Anyone running the loader WITHOUT canon judgment
(i.e. not through natura_examen) gets no layer-2 protection: that
is by design, not an oversight.

Deleted infrastructure that only served migrated rules:
ELEMENTA_NOTA/ATTRIBUTA_NOTA + in_literis (rule VIII),
machinam_probare whole (rule XII), claves_fontium table + its
build-time insertion (rule V), dies-format diagnostics (rule VII
format; dies_bene_formata SURVIVES as a guard on the ordering
check — comparing malformed dates means nothing).

probatio_natura: the vitiosa fixture's rule-VII fault changed from
a malformed date (now canon's) to an INVERTED interval
(valens_a="2020" valens_ad="1999") so the loader's surviving half
stays covered. Totals 15 → 11 vulnera.

## 2026-10-02 - cold-tree race: three tests building the natura tools at once

Found by the secunda session with tools/frigida_probare.sh: on a fresh
clone the radix suite failed a different natura test from run to run
("structor fefellit (codex 1)"). probatio_natura_glossae, _quaesitor and
_canones each run ./tools/natura_struere.sh if bin/natura_* is missing;
in parallel they compiled the same build/natura/*.o and linked the same
binaries at once. Reproduced with the OLD script run 8 at a time on a
cold build: 4 of 24 failed ("dsymutil ... opening bin/natura_canones: No
such file" - one copy's output vanished under another's linker). Three
copies on a quiet machine never failed, which is why it looked random.
Fixed in two layers:
- compile_tests.sh builds the tools ONCE before the parallel phase, when
  any selected test file mentions natura_struere.sh (derived from the
  selection, no hand list) - the tabulariumd precedent of 2026-09-02.
  With the binaries deleted, all four natura tests pass and none
  self-builds.
- tools/natura_struere.sh: house lock (tools/sera.sh,
  build/natura/struere.sera); links only when a binary is older than its
  objects, sources, provenance SOURCE (.c - the .o is always recompiled
  by provenientia_obiectum.sh, so its mtime says nothing), vexilla.sh or
  the script itself - always under FABRICA_AGIT (fabrica judges); links
  to <bin>.novum.<pid> then mv, so a test RUNNING the old binary is not
  killed by an in-place overwrite; the -g dSYM bundle moves with it (first
  version left 32 *.novum.*.dSYM behind). New script, 8 at a time: 40/40.
