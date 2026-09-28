# toml — TOML 1.0 as a materia client (seventh)

**Status: building** (plan `project-specs/toml-arbor-plan.md`, Q1 done).
Spec: `project-specs/toml-arbor-spec.md` (decisions T1–T11).

A total, byte-exact TOML tree on materia, a cooked view of typed values,
every error located; proven by three oracles. Replaces `lib/toml.c`
(retired in Q12).

## Oracles (Q1)

- `probationes/fixa/toml-test/` — toml-test v2.2.0 (MIT), frozen;
  `PROVENIENTIA.md` has tag, commit, counts (1.0.0: 205 valid, 474
  invalid) and the measured `tomllib` agreement (total).
- `instrumenta/tomllib_tagatum.py` — Python `tomllib` → toml-test tagged
  JSON; `./toml/tomllib_aurum.sh` writes `probationes/fixa/tomllib/aurum.txt`
  (committed, toml-test) and `build/aurum_silvestre.txt` (wild, NOT
  committed).
- `probationes/fixa/silvestria.manifestum` — the wild corpus as sha256 +
  path only (Fran 2026-09-28: no third-party files in the repo); 1,795
  unique files from `~/.cargo`, `/opt/homebrew`, `~/Documents/projects`.
