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

## 2026-11-10 — directorium_fontes_ambulare; ten header walkers converted

The shared source-tree walker, on top of PRAETERMITTE:
`directorium_fontes_ambulare(via, suffixum, praetermittenda, functio,
ctx, piscina)` calls the function for every non-directory whose name
ends in the suffix; dot names are skipped, an entry named in the
NIHIL-terminated list is skipped (a directory: its whole subtree), a
symlink to a directory is not entered. Tests: section "ambulatorem
fontium" (no list 4 files; list prunes `deep`; a symlink to sub1 is
not entered; empty suffix; early stop; unopenable root -1). Plants via
silva.planta, each red then green: symlinked directory entered; list
ignored; suffix ignored.

Before converting, the main risk was checked directly (scratch
program, old readdir loop against the new walker over this tree): the
same 777 headers in the SAME ORDER with identical paths ("./x/y.h").
Order matters - the tools key headers by basename, first wins, and 11
basenames occur more than once. Cost of the walk alone: 0.29 -> 0.30 G
instructions, 0.02 -> 0.04 s, +5 MB (paths built in the pool).

Converted (`_capita_praeparare` -> adapter `_caput_visum` + one call;
`_praetermittendum` -> the array CAPITA_PRAETERMITTENDA, kept as a
function over the array only where another walker in the file still
uses it): officina cursor, fusor, interpretare,
probatio_officina_vindex; silva census_ambigui, emitte, infidelis,
origo, quaestio, selecta. All ten had the same four names (build,
.git, results, node_modules). `<dirent.h>` dropped where nothing else
needs it. Source lists regenerated (tools/silva_fontes_generare.sh and
the three officina snippets): + iter_directoria, via, lectiones where
missing.

Differential: 18 invocations (selecta, emitte, origo, quaestio incl.
-invariantia and -omnibus over lib, infidelis, interpretare, cursor
-enumerare and -sola, fusor lib), stdout + stderr + exit code, before
(run twice: only two timing lines vary) and after: identical except
fusor's `tempus:` line. Binaries checked by nm: the walker is linked,
`opendir` gone from selecta/emitte/origo/infidelis (quaestio, cursor,
fusor, interpretare keep it for their OTHER walkers - next step).
With FABRICA_LECTIONES set, `emitte` now writes 979 'D' lines where
it wrote none: the enumeration is visible to fabrica.

Found: silva/instrumenta/principalia/census_ambigui.c has NO launcher
and no build rule (silva/ambigua.sh builds ambigua.c) - converted and
syntax-checked by hand only (clang -fsyntax-only, house flags). Its
fate (delete or wire up) is Fran's.

Left for the next steps: the 5 other `_capita_praeparare` copies
(examen, percursus, renominare, haruspex, censor/nexus_percursus/
identitates group - different skip lists and, in examen, a full-path
key), the other hand walkers, and the read-mode fopen inside
`_caput_praebere` (still raw - no 'L' lines yet).

## 2026-11-10 — census_ambigui.c deleted

Fran's ruling the same day: silva/instrumenta/principalia/
census_ambigui.c is removed (the M0 throwaway of 2026-07-08, its own
header said so; replaced by ambigua.c + silva/ambigua.sh on
2026-08-19; no launcher, no build rule, compiled by nothing). Its row
leaves the legacy read-debt list (canalis_debita.tsv). The design doc
and phase-log mentions stay as history.
