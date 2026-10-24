# iter_directoria.c worklog

## 2026-10-22 — single-call enumeration on macOS (getattrlistbulk) + tempus_ns / est_nexus

Motivated by fabrica's write-detection snapshot: it was 49% of the time of rebuilding radix members, walking ~57-84k entries twice per member with readdir + stat() (here) + lstat() (in the snapshot) + another lstat() for directories.

- `DirectoriumIntroitus` gains `tempus_ns` (mtime ns) and `est_nexus` (the entry ITSELF is a symlink). `genus` / `mensura` / `tempus_ns` keep the FOLLOWED (stat) semantics they always had; a broken link is still IGNOTUS. `mensura` is st_size even for directories (that was already the behaviour; the header comment said "0 pro directoriis", which was wrong and is now corrected).
- `#ifdef __APPLE__`: `getattrlistbulk` into a 32 KiB malloc'd buffer (freed in `claudere`; NOT from the piscina, since recursive walks keep thousands of iterators alive), requesting name, objtype, modtime and dir/file datalength. Symlinks (and any entry without a returned length) fall back to stat(), which preserves the followed semantics. Silva does not examine this arm (Darwin vernacular, like uuid.c / machina_posix.c).
- Portable arm (`#else`, which silva examines): readdir + stat as before, plus `est_nexus` from d_type (lstat only when DT_UNKNOWN). The silva model gained S_IFLNK / S_ISLNK / DT_UNKNOWN / DT_LNK (auspex certifies them). It also compiles with `-U__APPLE__`.
- Equality (probe `build/sonda_partes/sonda_aequalitas.c`): bulk attributes match lstat for 349,450 entries (repo with thesaurus, /tmp, /private/var/run): mtime ns and size, including 27,227 directories and 32 sockets/fifos; 0 differences.
- Speed: cold ~5x / warm ~2x for the iterator alone. The snapshot walk in its new form (iterator fields, lstat for links only) against the old one, interleaved under load: cold 1.60 → 0.39 s, warm ~0.5 → ~0.28 s, the same 51,730 files with an identical mtime checksum (`sonda_photographia.c`).
- Tests: probatio_iter_directoria "enumerationem uno vocamine": a temp tree with a file, dir, link to file, link to dir, broken link, fifo, a 200-char name and 1500 files (several buffer refills). Every field is checked against lectiones_stat. Plants (mtime without ns; links not followed; stop after the first buffer) are all red on their assertion, then green.
