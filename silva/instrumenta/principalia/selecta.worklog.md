# selecta worklog

## 2026-07-22 — birth (parcum 01KY53MP5X, same-day build)

Structure-shaped grep over one file: selector -> byte-exact
extents with [via:linea-linea] headers. Notes:

- **Extent measurer is emitte's, const-safe variant**: quaestio
  results carry `constans SilvaNodus*`; seeding the worklist with
  the subject's LOCI (SilvaValor copies) instead of a node valor
  avoids every const cast. If emitte's extent logic changes,
  change both (comment cross-refs).
- **-intra is a post-filter, not a walk**: ancestor climb +
  congruit("definitio-functionis:definit(sym)"). The original
  2026-07-11 sketch wanted emitte's binding-table walk — QC's
  :definit dissolved it. Seam law receipt.
- **The tool was right, the tester was wrong, twice**: -intra
  xar_obtinere found nothing because xar_obtinere genuinely never
  touches ->segmenta (goes through xar_locare); text-grep says
  "segment" 120 times in xar.c, the structural query says exactly
  2 accesses inside xar_locare. That asymmetry IS the demand
  receipt (M3 grep '->bloccus') paying out.
- **Pipeline-exit trap bit during testing**: `cmd | head; echo $?`
  reports head's exit. Gate on selecta's exit via redirect-to-file
  or PIPESTATUS (debugging-lessons law, re-proven on myself).
- Tree-with-errors = CAUTIO + continue (error nodes queryable);
  selector rejection = named causa + exit 2; exit grammar
  grep-parity (0 hit / 1 none / 2 fracture).
- Birth gate: `-proba` (embedded fixture, 5 exact counts + bad
  selector rejected). Run it after touching this file.
- Named futures: capture printing ($nomen per match), multi-file
  args, -breviter (headers only), examen-lexicon dirent (would
  un-exclude this file + emitte + censor...).

## 2026-10-10 - selecta.sh relinked on every call (ledger …WB6Q3H)

`selecta.sh` compiled+linked selecta.c into silva/build/selecta on EVERY
call (only the library objects were cached), then exec'd it. Two costs:
- concurrency: a second call exec'd the binary while the first rewrote it.
  Running tools/credo_convertere.py 6-wide made a concurrent credo fumus
  fail in 10 places (alone: green);
- time: 0.58 s per call, most of it the link (the converter makes ~15).

Now `tools/instrumentum_nectere.sh` (shared, sourced): relink only when
stale - binary missing, or an input STRICTLY newer (source, objects,
tools/vexilla.sh, the calling wrapper; header via the caller's
newest_header). Strictly: /bin/bash `-nt` compares SECONDS, and objects
rebuilt in the same second as the link made a "binary not newer than every
input" test relink forever (the fumus caught that on the first green
attempt). Link goes to `<bin>.nexus.<pid>` then `mv -f` (a running binary
is never rewritten in place); clang -g writes `<out>.dSYM` next to the temp
name when compiling+linking one source, so the dSYM moves too (otherwise
19 `selecta.nexus.*.dSYM` dirs piled up per stale burst). 0.25 s per call.

Gate `nexus-instrumentorum` (tools/instrumenta_nexus_fumus.sh): per wrapper
in INVOLUCRA - fresh call does not relink (inode+mtime equal), touched
source relinks, 8 concurrent fresh calls identical, 8 concurrent STALE
calls identical with an intact binary and no temp leftovers. Plants by hand
(planta mangles shell): always-stale, sources ignored, dSYM left - each red
at its named check. The other 18 wrappers with the same unguarded link
(examen, formator, quaestio, arbor, censor, ...; officina cursor/fusor/
interpretare) are the follow-up: add each to INVOLUCRA, switch to the
helper.
