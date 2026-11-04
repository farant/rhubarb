# tools/vexilla.sh — worklog

## 2026-11-02 — budget B2 J10: sourcing without subprocesses

vexilla.sh is sourced ~1300 times per merge (runners, wrapper scripts,
probatio_una) and cost ~11 ms each over a bare `bash -c :` (16.7 vs
5.4 ms): `$(cd "$(dirname …)/.." && pwd)`, `$(printf | cksum | cut)`,
`$(cat sigillum)`. Now:

- root from BASH_SOURCE by suffix strip when it is absolute and ends in
  /tools/vexilla.sh; anything else (relative, odd form) falls back to the
  old cd/pwd. A non-canonical absolute path still works (paths only).
- the signature is read with `read -r` (builtin), `cut` replaced by
  `${h%% *}`.
- cksum of the table is a PURE function of the table, so it is cached in
  the exported VEXILLA_CKSUM as `hash TAB joined-table`; a child reuses
  it only when the joined table matches exactly, otherwise recomputes.
  The signature FILE format (cksum) and the latina.h touch rule are
  unchanged — briar has its own sigillum code and aedificatio.stml
  declares the file as a shared trace, so the format must not move.

Measured (load ~8): cold 16.7 -> 9.6 ms, child with cache 16.1 -> 5.8 ms
(bare bash 5.3). ~10-14 s per merge.

The vexilla gate never tested the signature/touch logic. New section VII
on a scratch copy: first sourcing touches latina.h and writes the
signature; a re-source does not touch; a changed table WITH the old
VEXILLA_CKSUM exported still touches; relative sourcing finds the same
root. Plant (cache reused without the table check) -> the "tabula
mutata" case FRACTUM; restore green.

Test trap: macOS `date -j -f %Y%m%d%H%M` fills the unspecified SECONDS
from the current time — compare mtimes against a reference file touched
with the same `touch -t`, never against a computed epoch.
