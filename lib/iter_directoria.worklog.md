# iter_directoria.c worklog

## 2026-10-22 — single-call enumeration on macOS (getattrlistbulk) + tempus_ns / est_nexus

Motivated by fabrica's write-detection snapshot: it was 49% of the time of rebuilding radix members, walking ~57-84k entries twice per member with readdir + stat() (here) + lstat() (in the snapshot) + another lstat() for directories.

- `DirectoriumIntroitus` gains `tempus_ns` (mtime ns) and `est_nexus` (the entry ITSELF is a symlink). `genus` / `mensura` / `tempus_ns` keep the FOLLOWED (stat) semantics they always had; a broken link is still IGNOTUS. `mensura` is st_size even for directories (that was already the behaviour; the header comment said "0 pro directoriis", which was wrong and is now corrected).
- `#ifdef __APPLE__`: `getattrlistbulk` into a 32 KiB malloc'd buffer (freed in `claudere`; NOT from the piscina, since recursive walks keep thousands of iterators alive), requesting name, objtype, modtime and dir/file datalength. Symlinks (and any entry without a returned length) fall back to stat(), which preserves the followed semantics. Silva does not examine this arm (Darwin vernacular, like uuid.c / machina_posix.c).
- Portable arm (`#else`, which silva examines): readdir + stat as before, plus `est_nexus` from d_type (lstat only when DT_UNKNOWN). The silva model gained S_IFLNK / S_ISLNK / DT_UNKNOWN / DT_LNK (auspex certifies them). It also compiles with `-U__APPLE__`.
- Equality (probe `build/sonda_partes/sonda_aequalitas.c`): bulk attributes match lstat for 349,450 entries (repo with thesaurus, /tmp, /private/var/run): mtime ns and size, including 27,227 directories and 32 sockets/fifos; 0 differences.
- Speed: cold ~5x / warm ~2x for the iterator alone. The snapshot walk in its new form (iterator fields, lstat for links only) against the old one, interleaved under load: cold 1.60 → 0.39 s, warm ~0.5 → ~0.28 s, the same 51,730 files with an identical mtime checksum (`sonda_photographia.c`).
- Tests: probatio_iter_directoria "enumerationem uno vocamine": a temp tree with a file, dir, link to file, link to dir, broken link, fifo, a 200-char name and 1500 files (several buffer refills). Every field is checked against lectiones_stat. Plants (mtime without ns; links not followed; stop after the first buffer) are all red on their assertion, then green.

## 2026-11-10 — the walker callback can skip a subtree (PRAETERMITTE)

Why: the read-side sweep (budget fusionis, Track B) found 48 raw
opendir sites; 15 are one copied function (`_capita_praeparare`, the
recursive header walker of the silva and officina tools) and 22 files
carry a copy of its skip rule (`_praetermittendum`: build, .git,
node_modules, ...). They were hand loops because directorium_ambulare
could only continue or stop the whole walk - it could not say "do not
descend into this directory". Fran's ruling: the skip belongs in the
house walker (not a silva-side helper, not a lectiones_opendir
wrapper).

Change: callback answers are named. DIRECTORIUM_AMBULATOR_PERGE (0),
DIRECTORIUM_AMBULATOR_PRAETERMITTE (II): for a directory, its subtree
is not entered (never opened, so no 'D' line in the lectiones book);
for a file it means PERGE; it is never returned to the caller. Any
other non-zero value still stops the walk and is returned. All 14
existing callbacks return only 0 or 1 (audited), so nothing changes
for them.

Limits to remember when converting the tools:
- the callback sees a directory only if the filter lets it through
  (a genera list without INTROITUS_DIRECTORIUM cannot prune); the
  name pattern only applies to files, so `exemplar = "*.h"` with no
  genera list is the shape for the header walker;
- dot directories without includere_occultos are neither reported nor
  entered (same as the hand loops, which skip dot names);
- genus is the FOLLOWED status: a symlink to a directory is an
  INTROITUS_DIRECTORIUM and the walker descends, where the hand loops
  (d_type == DT_DIR) do not. The callback can answer PRAETERMITTE on
  `est_nexus`;
- enumeration order comes from getattrlistbulk, not readdir - each
  converted tool needs an output differential.

Tests: section "subarborem praetermissam" (prune sub1: itself seen,
nothing under it, 8 of 11 entries, walk returns 0; prune the deeper
`deep`: 10 of 11; PRAETERMITTE on files: all 11). Plants via
silva.planta, each red then green: the subtree is entered anyway
(l.574); PRAETERMITTE treated as stop (walk returns 2, l.571).

Census of the copied skip rule (22 files), for the conversion:
- 12 files: .git build node_modules results (officina praeparator,
  cursor, fusor, interpretare, two officina tests; silva
  census_ambigui, emitte, infidelis, origo, quaestio, selecta)
- 5 files: .git build node_modules (silva examen, renominare,
  silva_iudicium, two silva tests)
- 2 files: + censoris, results (censor, nexus_percursus)
- 1 file: + vendor (identitates)
- 1 file: build node_modules + a prefix rule (transformare)
- percursus: not extracted by the script (read by hand when converted)
- haruspex has the walker but no skip rule.
Whether the differences are deliberate is decided per tool by the
output differential, and brought to Fran where output changes.
