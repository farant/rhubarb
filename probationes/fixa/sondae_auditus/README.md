# sondae_auditus — repro probes from the library audit (2026-10-17..19)

Each `<unitas>_*.c` is a scratch harness written by the audit fleet to
reproduce one confirmed defect (or a group of them) in a house library.
They are the evidence behind the open parks under ledger parent
01M56KN2H67CEEZ5FMG4W316A9 (and its children), which cite them by
name. Copied here from the worktree's gitignored `build/sonda/` so the
repros outlive any one checkout.

- **Fixtures, not suites:** nothing builds, runs, formats or lints them.
  Paths and APIs are as of 2026-10-19.
- **Run one:** `./tools/sonda.sh -domus probationes/fixa/sondae_auditus/<via>.c`
  (house flags, private build, memory and time guard; `-nudus` for
  sanitizers off). The `.h`/`.inc` files are shared helpers some probes
  include.
- **When a park is fixed,** turn its probe into a credo section in the
  library's own suite (red before the fix, green after) and delete the
  probe here.
- Probes for `arbor*` target code now frozen in `archivum/` (see
  `archivum/CLAUDE.md`). They are kept as the record of those defects and
  will not build against the live tree.
