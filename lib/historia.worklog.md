# historia worklog

## 2026-10-05 — H1: the engine, extracted (scriba-plan H1)

`historia` is pictor_documentum's log engine with the pixels taken
out: acts in a volumen, branches, checkpoints, cursor, undo/redo,
verify. The client gives a `HistoriaProiectio` (fixed-size memory it
owns + `vacare` + `applicare`), the genus of its acts and the origin
label of its checkpoints. The manifest (dimensions, interval) stays the
client's business - historia writes nothing to the volume except acts,
`<ramus>` and checkpoints. The code is a line-for-line translation of
the engine half of `lib/pictor_documentum.c` (the H0 golden will hold
H2 to that).

**What `verificare` really checks** (my first test expected otherwise):
it compares the sigillum RECORDED after the last applied act with a
replay from nothing. So it catches an incremental projection that
diverged from the log *and was signed* (a corrupt state that a
following act or checkpoint sealed) - e.g. pictor's ramus-ignored plant.
Memory scribbled on AFTER the last signature is not detected: the
recorded sigillum is still the right one, the replay matches it and
silently repairs the memory. Named in the header; test VII pins the
silent repair, XI the signed divergence.

**The toy projection counts its calls.** Eight i8 counters, act "k"
increments counter k; the context counts `applicare` calls. That is
what makes "undo starts from a checkpoint" observable: with checkpoints
ignored the RESULT is still right (only slower), so only the call count
shows it (plant P4).

Plants, all compiling (exit codes read): no `<ramus>` after undo (P1:
dead act revived, sigillum after redo differs); a dead checkpoint
admitted as base (P2: undoing past a branch revives an undone act);
acts at or before the checkpoint re-applied (P3: counters double);
checkpoints never used (P4: call counts only). My first P2/P4 runs did
not compile (sed-style `\&` left in a Python replacement - a literal
backslash in C): the A3 lesson once more - read the build line, not
just the FRACTA lines.

Link closure: a new lib needs `./tools/compile_tests_fontes_generare.sh`
(generated list; the `generata` gate judges it).
