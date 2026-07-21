# aedilis — spec v2 (2026-07-20)

*Status: post-exploration; supersedes aedilis-spec-v1.md where they
differ. Sources: two explorer reports (silva amalgam surface;
stml/argumenta/cache/consumer plumbing) — claims below are file:line
verified by them. Interview: aedilis-interview.md. Park: 01KXJ2HV.*

## I. Amendments & decision-revisits (what exploration changed)

1. **Q5 REVISIT — the incremental graph cache dissolves.** The
   interview chose "link silva + incremental cache" on the premise
   that parsing a closure costs seconds. Measured reality
   (phase-log.md:1498-1500): the FULL pipeline runs at 0.299 ms/KB —
   a 30KB lib file ≈ 9ms, the whole repo sub-100ms. A fixpoint
   closure (~10-60 files) parses in ~10-30ms fresh. RECOMMENDATION:
   **parse fresh every run** — zero staleness machinery, zero cache
   format, structurally always-true. The "link silva" half of the
   decision stands; the cache half was solving a problem that does
   not exist. (If a future giant repo changes the math, the
   GENERATUM-stamp + .nova-rename idiom from nexus_percursus.c:1560,
   1014 is the pattern to copy.)
2. **build/inclusiones.tsv exists** — a repo-wide include-edge cache
   (written by nexus_percursus.c, served verbatim by legati
   inclusiones, legatus.c:6149-6176). Aedilis does NOT consume it
   (fresh parse is cheaper than trusting any cache), but
   differentia-clausurae MAY cross-check against it as a free third
   witness. One graph-sweep family, aedilis independent of it.
3. **Include semantics are POST-evaluation — and that is the RIGHT
   semantics for linking.** silva records include directives only in
   taken #ifdef arms (silva_expandere.c:813-823; empty macro env by
   default; ordinary include-guards ARE taken on first pass, so
   normal headers all appear). This matches what the compiler
   actually sees under the actual build — for LINK closure it is
   arguably truer than -MM's both-arms superset. House
   file-per-platform convention makes divergence rare; where it
   exists, differentia shows it as ORACULUM SOLUM entries to be
   classified. Escape hatch if ever needed: RAMUS_OMISSUS raw
   slices (silva.h:997) expose untaken arms. DECISION: post-eval
   primary; differentia adjudicates; no all-arms walking in v1.
4. **Annotations need NO parse at all**: silva_lexare_cruda
   (silva.h:252) interposes comment tokens inline in source order —
   scan each closure file's COMMENTUM tokens for "aedilis:" at
   0.335 ms/KB. Same lexer, cheapest possible channel.
5. **Platform vocabulary correction**: `-machina` is examen's
   machine-OUTPUT flag, not a platform word (examen.c:188-190).
   Aedilis reuses `-machina` for TSV/machine output (consistency)
   and introduces **`-variantes <verbum>`**… no — simpler: variant
   selection defaults to macos and is configured in aedilis.stml
   (<variantes> praelatio order); a `-varians posix` flag overrides.
   `-posix` stays reserved for the lexicon sense to avoid aliasing
   examen's meaning.
6. **Aedilis cannot ride the generic tools loop**: silva.o + 
   -Isilva/amalgama are not in compile_tools.sh's path (it links
   build/*.o with -Iinclude only). Aedilis gets a dedicated
   `tools/aedilis_struere.sh` mirroring nexus.sh:32-66 (compile
   silva.o if stale, rhubarb libs, aedilis.c, link). The amalgam is
   self-contained (system includes only, prefix-renamed internals —
   silva.c:8-38) so no collision with rhubarb objects.
7. **STML write API is real** (stml.h:290-412): element/attribute
   construction + stml_scribere(nodus, piscina, pulchrum) +
   _ad_aedificator. Everything interned + piscina-backed — aedilis
   carries an InternamentumChorda. SPIKE ITEM: verify attribute
   escaping for `"`/`<` in stml_scribere before trusting emitted
   manifests (explorer could not confirm).

## II. Architecture (concrete, with real API names)

### A. Build: tools/aedilis_struere.sh
Mirror nexus.sh: stale-check + compile silva/amalgama/silva.c →
build/aedilis/silva.o (first compile is the one slow step; cached
after); compile tools/aedilis.c with -Iinclude -Isilva/amalgama;
link with rhubarb objects (piscina, chorda, chorda_aedificator, via,
xar, stml, internamentum, tabula_dispersa, filum, argumenta,
flatura?) — explicit list in the script (aedilis will one day emit
its own; noted, not required).

### B. Engine loop (tools/aedilis.c)
For each pending source file (seed = target):
1. filum_legere_totum → bytes.
2. `silva_c89_parsare(sp, via, fons, mensura, NIHIL)` on a fresh
   SilvaPiscina per file (nexus_percursus.c:369 pattern — create,
   read expansio, destroy/vacare).
3. Enumerate `silva_inclusiones_numerus/silva_inclusio_vista`
   (silva.h:697-698): each vista.via = requested path text.
   RESOLVE OURSELVES (spec-v1 §III stands): quoted → via_iungere
   (dirname of includer, requested) → via_normalizare; then the
   -I list from aedilis.stml; angle → system unless it resolves in
   -I paths. House hit → pending headers set; system → recorded;
   vendor/ → vendor section.
4. `silva_lexare_cruda` scan of the same bytes for COMMENTUM tokens
   containing "aedilis:" → obiectum/vexillum declarations.
5. Header→object: include/X.h → probe lib/X.c, lib/X_posix.c,
   lib/X_<varians>.c, lib/X_<varians>.m per <variantes> order;
   irregulars from aedilis.stml (and fabrica.tsv obiectum species,
   read-only — awk-trivial TSV, excubitor.sh:81-93 grammar).
6. Discovered .c → step 1. Discovered .m → **-MM minority lane**:
   system("clang -MM <inclusa> file.m > build/aedilis/tmp") and
   parse (the blessed awk shape ported to C or a tiny popen-free
   temp-file read); entries marked origo="oraculum".
7. Fixpoint → attach flags (base from aedilis.stml; per-object
   nexus rules → frameworks; vendor flag sets; annotation vexilla)
   → manifest.
FAILURE: any unreadable/unparseable file, unresolvable include, or
headerless object claim → REFUSE LOUD (named file + cause, exit 1,
no artifacts).

### C. Artifacts (build/aedilis/<basis>/ — gitignored by blanket
build/ rule)
- `manifestum.stml` — root <aedilis-manifestum scopus=... machina=
  ... generatum=<epoch> commissum=...>; sections <obiecta> (obiectum
  via/fons/origo where origo ∈ derivatum|aedilis.stml|fabrica|
  annotatio|oraculum), <capita>, <systema>, <vendor>, <nexus>
  (vexilla with origo), <exclusa/lacunae never — refusal instead>.
- `struere.sh` — cache-reusing: per-object mtime compile-if-stale
  (the newest_header idiom, colloquium.sh:40-44) then link.
- `struere_solitarius.sh` (on -solitarius) — hermetic full compile.
Scripts bash-3.2-safe, exit-code gated, no ANSI.

### D. aedilis.stml (committed, repo root)
```
<aedilis>
  <inclusa><via valor="include"/><via valor="probationes"/>
    <via valor="book_assets"/> ...(the 5 from compile_tests)</inclusa>
  <vexilla>-std=c89 -pedantic ... (the house set)</vexilla>
  <nexus>
    <regula obiectum="fenestra_macos" vexilla="-framework Cocoa"/>
    <regula obiectum="tls_macos" vexilla="-framework Security"/>
    <regula obiectum="vitrea_macos"
            vexilla="-framework WebKit -framework Cocoa"/>
  </nexus>
  <vendor><fons via="vendor/sqlite3.c" vexilla="-O2 -DSQLITE_..."/>
  </vendor>
  <variantes praelatio="macos"/>
</aedilis>
```
(Schema firmed in Phase A; this is the shape.)

### E. differentia-clausurae
`bin/aedilis -differentia <file>`: silva-lane closure vs clang -MM
closure; buckets CONSENSUS / NOS SOLI / ORACULUM SOLUM; `-census`
sweep over all probationes for the corpus gate. Optional third
witness: build/inclusiones.tsv cross-check (report-only).

### F. Consumers (explorer-verified seats)
- gesta: emit `gesta/fontes_generata.sh` containing
  `RADIX_FONTES=( ... )`; compile_probationes.sh sources it in
  place of the literal array (declare-then-iterate structure is
  cleanly source-able; no source idiom exists yet in the repo —
  this is the first, precedent-setting).
- Bench: colloquium.sh/interpretare.sh pass -plagulae through to
  the C binary (a substring FILTER over lib/*.c, interpretare.c:
  303-310, not a link list) — wrapper-side derivation: when the
  user passes no -plagulae, the wrapper may call bin/aedilis on the
  implied TU and append a derived filter list to "$@" (seat (b),
  zero C changes). Marked EXPERIMENTAL in this arc: the filter
  semantics (substring) vs closure list needs one calibration look.

## III. Phases (firm)

- **0 — spike**: aedilis_struere.sh; minimal aedilis.c that parses
  ONE file and prints directives + aedilis: comments + timing;
  stml write round-trip INCLUDING attribute-escaping specimens;
  via_normalizare specimen pins ("foo/../bar", "../x", "a//b").
  GATE: printed directives for lib/hospitium.c match its 13 known
  includes (this session's inclusiones probe as fixture).
- **A — engine**: resolution + convention/variant mapping +
  channels (aedilis.stml, annotations, fabrica obiectum) + fixpoint
  + manifest emission + refusal; probatio with fixture specimens
  (platform variant, annotation obiectum [speculum case], vendor
  coupling, unresolvable → refusal).
- **B — emitters + oracle**: both script postures; .m minority
  lane; differentia-clausurae + corpus sweep; THE GATE: every
  probatio built via its emitted script, all green + consensus.
- **C — consumers + seal**: gesta snippet regeneration; -plagulae
  wrapper derivation (experimental flag); worklog; board RELATIO.

## IV. Remaining open items

1. stml_scribere attribute escaping (Phase 0 spike).
2. silva.o first-compile wall time (measure in spike; cached
   thereafter).
3. Whether silva_lexare_cruda behaves acceptably on .m bytes for
   comment scanning (lexical C-ish; if not, .m annotations via
   aedilis.stml only — tiny loss).
4. -plagulae derived-filter semantics calibration (Phase C).
5. Naming inside the manifest for the emitted scripts' provenance
   block (mirror speculum's proventus fields).
