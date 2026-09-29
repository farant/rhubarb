# runae — worklog

## 2026-09-28: U1, the Lapide corpus

- Lapide's language suffixes: the unsuffixed page is ENGLISH and `_lt` is
  LATIN (by the pages' own `hreflang`). The plan's first draft had both
  wrong; fixed there.
- The house HTML parser (materia's html client) extracted all 35
  languages, and an independent Python stdlib extraction under the same
  normalization matched byte for byte. The walk is simple because
  Lapide's pages are clean: no content out of place (`sedes`), no
  adoption agency. A messier page would need html_coctum's ordering
  logic, so don't reuse the walk on arbitrary HTML.
- Useful for later tasks: `ar` is fully vowel-marked (dense combining
  marks, width 0), `yo` stacks combining tone marks on dotted letters,
  `ja` uses U+3000 ideographic spaces (kept, not collapsed), `th` has no
  spaces between words.
- The tool builds against `html/build/*.o` (like `html/arbor.sh`); those
  objects have no `filum`/`utf8`, so it uses stdio directly.
- The script gate proves the script, not the language: Latin-script
  languages (21 of them) and ar/fa can't be told apart by it.
- examen: `<br>` inside a C comment is read as an annotation tag; I wrote
  "(br)" instead.

## 2026-09-28: U2, the width table

- The width rule is Ghostty's, read from its code (the vendored tarball
  in opentui: `ghostty/src/build/uucode_config.zig` + `uucode-ghostty/
  src/components.zig` `Wcwidth`), not from my research note, which had
  it slightly wrong (Mc isn't 0; emoji presentation isn't its own
  rule). Next time, read the code before stating a rule.
- `DerivedCoreProperties.txt` has a SECOND `@missing` line mid-file (for
  InCB, line 12607), after DI's data. A blanket "@missing must come before
  data" guard rejected the file; the guard now fails only when the late
  default touches flags we actually read. U4 reads InCB and must apply
  that default BEFORE InCB's data lines, i.e. per property, not per
  file.
- EastAsianWidth 15.1 lists the default-W unassigned ranges (plane 2/3,
  CJK blocks) explicitly; its only `@missing` is `N` for everything.
- The table: 104 unique 256-byte blocks (26,624 bytes) + 4,352 u16 block
  indices = ~35 KB of data; the generated source is 76 KB.
- `-probare` ignores whitespace: the commit-time formator may reflow the
  committed generated file (the same reason silva's lexicon check does;
  the tessera amalgam was bitten by exactly this in 1.2 T5). Plant:
  whitespace-only change → still fresh; a value change → stale.
- `tools/generata_probare.sh` section VI now runs `-probare`. In the
  secunda worktree the gate as a whole still reports the silva/officina
  manifests stale (a worktree-only false alarm), so I ran section VI by
  hand for the commit.
- Corpus measurements: ar has 928 zero-width runes out of 2,343 (full
  vowel marking); hi 310, th 328, yo 164; ja/zh have almost no
  width-1 runes, ko 489 (its spaces and punctuation).
- The Latin lint blocked the first commit on 6 new words. `graphema`
  (a house term like `lexema`, heavily used from U4 on) and `ignorabilis`
  (classical) went into `oratio/glossarium.stml`. `gc` became
  `V_CATEGORIA_NULLA`, `iamo` became `V_MEDIAE_FINALES` (Hangul medial and
  final letters), and `RUNAE_UNICODUM_VERSIO` became `RUNAE_VERSIO`.
