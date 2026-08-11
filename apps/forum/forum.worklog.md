# forum.c worklog

## 2026-08-11 — charta step 1: censum_legere (census graph serializer)

Spec: project-specs/charta-spec.md. New CHARTA section in forum.c:
`censum_legere {signum?}` → graph payload / `{mutatum:false}` /
`{vitium}`. Registered in the public method list (phone gets it free).
First real fumus: **92 nodi, 153 aristae, 68 glossae generum,
0 praetermissa** — 92 = 84 census entities + the `<% &c;>`
augmentation + 7 external stubs, which cross-checks.

Decisions and traps worth remembering:

- **signum = `mtime.size.inode` per file, three files joined.**
  Inode catches atomic saves landing within the same mtime second
  (write-temp-then-rename mints a new inode) — mtime alone would
  miss them.
- **individua.canon (846KB) parses into its own throwaway piscina**
  (8MB, created/destroyed inside `_charta_glossas_apponere`), never
  into the 8MB piscina_vocationis — the known generated-file OOM
  bite (01KYQ4H5ZG). Gloss chordae are `chorda_transcribere`d into
  the request piscina *before* the canon piscina dies; forgetting
  that copy would be the classic dangling-chorda corrupted-response
  bug already documented at the `_sententias_parsare` sigillum
  comment.
- **`nomen` and `externus` are latina.h macros** — the struct
  fields/params are `clavis` and `alienus`; only the JSON string
  keys say "nomen"/"externus".
- **Augmentation blocks**: stml parses `<% &c;>` as an element
  titled `%` with `augmentum_clavis` carrying the citation
  verbatim — the node keys off that, marked externus.
- **ChartaNodus pointers don't survive `xar_addere` on the same
  xar** (growth relocates) — the code never holds one across a
  node creation; comment on `_charta_nodum_creare` says so.
- **Planted faults, both RED**: truncated census → `{vitium}` →
  `fumus charta FRACTUS: vitium` exit 1; empty-but-valid census →
  zero nodes → FRACTUS exit 1. Proven by running `bin/forum -fumus`
  from a scratch cwd containing the broken `rhubarb.census` (the
  charta paths are cwd-relative; the daemon connection is TCP so
  the rest of fumus doesn't care about cwd).
- **Edges carry `familia` only** — the familia→causa map is a
  static ~11-row table that belongs in the JS view next to the
  kind→stratum table (spec §2.5 records the reasoning). Avoided a
  third parse + underscore/hyphen normalization in C.
- The zsh `===` separator trap bit again during the planted-fault
  run (equals-expansion ate the second test) — quoted separators,
  always.
