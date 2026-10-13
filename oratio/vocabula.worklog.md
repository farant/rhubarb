# oratio/vocabula.sh - worklog

## 2026-10-12 - the index is refreshed before judging (circulus commissionis T3, P2)

- **Bug:** `-nova` (and `-symbola`/`-omnia`) read `build/nexus.tsv` as it
  stood, so right after an edit a new unknown word was invisible and the
  answer was a false `NOVA 0`. It bit 09-14, 10-05, and again in
  portae-declaratae T3, where the commit's pre-gate lint caught what the
  manual check had missed. Ledger 01M46ER35.
- **Fix:** every mode that reads identifiers (all except `-commenta` and
  `-prosa`) runs `./silva/nexus.sh -renovare` first: about 1 s
  incremental; a full sweep only when the tool or table changed. If the
  refresh fails, the exit is 2, because judging with a stale index would
  lie. Measured: a manual `-nova` takes 1.8 s end to end.
- The hook (tools/unci-git/lint_latinus.sh) already refreshed the index
  and still does, so it passes `VOCABULA_INDEX_RENOVATUS=1` to avoid a
  second sweep. The sweep stays in ONE place per call.
- **Gate:** unci fumus stage XXI writes a file with `xyzzyquux_nova` and
  runs `-nova` straight away; it must say NOVA 1. The plant
  (`renovandum=0`) turned it red.
