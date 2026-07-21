# speculum_generare.sh worklog

## 2026-07-21 — the generator drinks from aedilis

The closure now comes from the aedilis manifest instead of the
generator's own clang -MM over compile_tests' passed
link-everything lists. Contract shrank to three args (titulus,
app, fabrica) — the `-- c -- objc -- vendor` list-passing and
SPECULUM_INCLUSA are gone; run_speculum passes only
SPECULUM_VEXILLA (provenance record).

**The bundle became honest**: 303 → 80 plagulae, 9.2MB → 1.5MB
generated .c, 2s generation. Speculum's charter is the honest
rebuild set; the honest rebuild set IS the link closure, which
-MM-over-everything never was. Consequences the truth-pins had to
learn (probatio_speculum_fontium): count bounded both ways
(L..CC), no vendor and no >1MB entries in exclusa (hospes'
closure genuinely has none — those rows only existed because the
whole SOURCE_FILES world was swept), mandatum= now points at the
gate-proven aedilis script instead of a hand-assembled clang line.

**New meta**: speculum/manifestum.stml rides IN the capsula — the
mirror carries its own dependency manifest (DE tab can render it
someday). Generated (annotated) objects under build/ land in
exclusa as "generatum (annotatio)"; the quine keeps its own line.

Parsing the manifest = 10-line section-aware awk (-F'"' on the
via attribute) — bash-side consumption of the artifact of record,
no CLI extension needed.

Reminder relearned the hard way: -speculum-fumus CONFIRMS but the
app keeps running — GUI smoke plumbing law applies (timeout/kill,
never bare foreground). Also filtered suite runs regenerate the
capsula but do NOT relink the wearer (name doesn't match filter)
— a stale-binary fumus reads the OLD count; rebuild via
./compile_tests.sh vitrea_hospes before believing the number.

## 2026-07-20 — Phase 0 (generator + build wiring + probatio)

- **Staging+reuse design**: rather than teaching capsula_generare an
  output-dir option or emitting the blob ourselves, we COPY the closure
  into build/speculum/<titulus>/ (mirroring repo layout) and run the
  existing bin/capsula_generare on a generated toml there. Entry vias
  stay repo-shaped ("lib/piscina.c"), the shipped tool stays untouched.
- **clang -MM closure**: the blessed shape from gesta/tabularium.sh
  (one invocation, awk continuation-join). The system-header filter is
  simply relative-vs-absolute: house paths come out relative because
  the build invokes with relative paths; /usr/... is absolute. Fallback
  when -MM output is empty: FONTES + include/*.h superset — garrulum,
  numquam caecum.
- **SIZE CAP 1MB** (first run emitted a 53MB .c!): the honest rebuild
  set contains data-as-source monsters — capsula_libri.c 12.8MB,
  biblia_dr.c 6.1MB, arbor2_glr_tabula.c 1.5MB. Embedding them would
  ship already-embedded data twice. They go to exclusa (via, size,
  cksum, causa "magnitudo>1MB"); the largest hand-written source
  (arbor2_glr.c ~500KB) stays embedded. Result: 53MB → 9.2MB.
- **capsula_generare's glob matches DIRECTORIES too** (readdir with no
  d_type check) — per-dir globs produced 3 empty junk entries
  (probationes/hospes_assets etc. as entries). Fix: the toml lists
  explicit file paths, no dir globs.
- **Quine chicken-egg**: exclusa.txt rides INSIDE the capsula, so the
  emitted .c cannot list its own final size (size depends on content
  depends on size). Listed with magnitudo 0 + causa "generatum -
  continet hos ipsos fontes" per spec decision #17.
- **bash 3.2 landmine** (macOS /bin/bash): `set -u` + empty
  `"${arr[@]}"` = unbound-variable death. Dropped set -u, explicit
  guards instead.
- **SILVA LIMITATION FOUND**: a quoted include containing ".."
  ("../build/speculum/hospes/capsula_speculi_hospes.h") is NOT resolved
  by silva's includer-relative search (clang resolves it fine) →
  "identificator ignotus" FP on the extern symbol, REICE. Consumer
  pattern of record instead: do not include the generated header;
  declare `externus constans CapsulaEmbed capsula_speculi_<titulus>;`
  directly — the symbol name is the stable contract, capsula.h carries
  the type. Filed on the board for the silva lane.
- **Freshness gate**: find FONTES+include+lib+fabrica+self -newer
  output → skip. Provenance staleness between source edits accepted.
  Full regen ~4s + ~15s one-time compile of the 9MB .c; gate amortizes
  both to zero ("recens").
- Probatio: probatio_speculum_fontium.c, 31 asserts — apertio/count,
  closure presence (.c/.h/.m/app), gzip round-trip vs mensura_cruda,
  worklog presence+body, proventus/exclusa/fontes.txt content, scripta.
  Linked via a per-test extra-object case in compile_tests.sh (the
  generated .o lives in the staging dir, NOT build/ root — it must not
  auto-link into every binary).
