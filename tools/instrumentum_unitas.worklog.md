# instrumentum_unitas.worklog.md

## 2026-11-07 - natus: percursus built as one translation unit (budget item 10/11b)

Why: the parse-side census (silva/fontes/silva_parsare.worklog.md, item 9)
found xar accessors at 20% of self time - call overhead across .o files on
an O(1) path. ThinLTO recovered 337.5 -> 285.4 G; a unity build through the
knotapel snapshot tool (conflator) got 290.3 G in plain C89 (Fran: the more
robust route; first step of the "webpack for C" wish 01M510B2).

Shape: sibling of tools/instrumentum_nectere.sh with the same contract (the
nexus-instrumentorum gate checks it for percursus: fresh call does not
relink, touched source relinks, 8 concurrent fresh/stale calls all succeed
with no temporaries left). Closure from `bin/aedilis <root> --partes`,
cached in `<bin>.unitas/partes.tsv`, so the freshness check is one `find`
over the closure + vexilla + caller + helper + conflator sources. A stale
build takes a mkdir lock (dead owner broken via pid), runs aedilis, builds
the conflator, refreshes silva's nexus, writes the statics table
(symbola_tabulam_scribere, factored out of corpus_infixum.sh - corpus.symbola.tsv
byte-identical after the refactor), conflates; compiles outside the lock
into `<bin>.nexus.<pid>`, then mv. The conflated file stays in the cache
(handy to read; #line points back to the real files).

percursus.sh: unity by default; SILVA_UNITAS=0 = the old object build into
a SEPARATE binary (build/percursus_obiecta) so neither mode can run the
other's binary. Cold unity build ~11 s single core; fresh call 0.02 s.
percursus links 33 sources now (true closure) instead of every
silva/fontes/*.c.

Gotchas found on the way:
- clang -g compile+link in one step writes `<out>.dSYM` next to the
  temporary output; left behind 10 per concurrent run until moved with the
  binary (instrumentum_nectere already did this - copied).
- percursus' closure has NO duplicate statics (the silva amalgamator refuses
  them, S41), so building with an EMPTY statics table compiles clean and the
  report is identical. The gate counts renames (#undef >= 100) instead.
- silva.planta restores the planted file's mtime, so a binary built under a
  plant looks fresh afterwards: plant U4 first "survived" and left a
  plant-built binary installed. The gate now deletes the unity binary, its
  cache and the object binary and always builds from source (memory
  plant-restore-mtime already said this for fumus scripts).

Gate `unitas-percursus` (silva/unitas_probare.sh, ~30 s): both modes build;
the conflated file is real (#line for >= 30 files incl. xar.c and
silva_glr.c, renames applied, binary not older); touching a CLOSURE file
(silva_glr.c, mtime restored afterwards) relinks the unity binary;
`percursus -semantica lib` gives the same report and exit code both ways.
Plants 5/5 red: default back to objects; closure dropped from the freshness
check; silva/fontes missing from the roots; empty statics table; comparison
against the wrong output (by hand - planta's anchor lookup did not find
shell words in that line).

Measurement (whole repo, 2 interleaved runs, report identical): unity
290.9/290.6 G, objects 337.0/337.2 G (-13.7%).

Open: other silva tools (examen, formator, the LSP side) could opt in the
same way; each needs its own cache dir, and two tools' stale builds would
refresh silva's nexus concurrently (outside each other's lock).

## 2026-11-07 - rebuild bookkeeping trimmed (item 12): smaller win than estimated

Step timings of a stale rebuild (11.1 s): aedilis closure 0.40, conflator
freshness 0.15, nexus refresh 1.46 (with nothing changed), statics table
2.4 (awk + sort over all 520 k 'sedes' rows of the 200 MB nexus.tsv),
conflation 0.09, clang -O2 -g 6.6 (frontend 0.2 s; -O0 whole build 0.86 s;
the rest is the optimizer + codegen, instruction selection 14%,
InstCombine 8.5%, inliner 5.7%, GVN 4.8%...).

Two changes:
- statics table filtered to the closure (symbola_clausurae_scribere in
  corpus_infixum.sh): conflated percursus byte-identical to the full-table
  conflation; closure rows identical (6,427). But the floor is one pass of
  BSD grep over 200 MB (~1.3 s), so 2.4 -> ~1.6 s. `grep -F -f` with 72
  patterns under BSD grep took 1 min 44 s (zsh's `grep` is ugrep - my first
  timing, 0.1 s, came from there; scripts get /usr/bin/grep). awk alone
  over the file 4.3 s, grep -E alternation 3.0 s.
- nexus refresh skipped unless a closure file is strictly newer than
  build/nexus.tsv. Only helps rebuilds NOT caused by a closure edit (flags,
  helper, wrapper): an edited closure file genuinely needs the refresh.
  Caveat in the code: a file restored with an OLD mtime (cp -p, planta)
  after the nexus captured the plant is not seen.

Measured end to end: wrapper touched 11.2 -> 10.3 s; closure file edited
10.6 s. Tracing with a perl-timestamp PS4 adds ~1.4 s to the 72-path loop
(looked like a slow conflator build at first).

Real levers left: clang (chunked unity in parallel, or -O0 dev builds) and
the index format (nexus.sh writing its 'sedes' rows - 37 MB - or per-file
statics, so the table need not scan 200 MB).
