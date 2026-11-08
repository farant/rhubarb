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
