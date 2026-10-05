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

## 2026-10-05 — joined acts (scriba-plan S1b)

For scriba's debounced insert (Fran: one `u` undoes the WHOLE insert,
however many ~1 s chunks it was saved in). `historia_actum_coniunctum`
appends a historia-owned marker (`<coniunctio/>`, genus "coniunctio")
right before the act - after any `<ramus>` - so the reader carries
"the next act of my genus is joined" without knowing its seq; volume
bookkeeping acts in between are skipped as before. No marker when
there is no live act to join to.

`acta_viva` returns a parallel Xar of b32 flags (optional out
parameter); a `<ramus>` truncates acts AND flags. `revocare` walks back
from the cursor's act to the start of its group and lands on the act
before it (`numerus_vivorum` = that index - recounted, not
decremented); `reficere` applies the next act and keeps going while the
following one is joined. Checkpoints and branches are unchanged.
H0 golden byte-identical (pictor never joins).

**A plant survived the first test** - truncating acts but not flags.
After a branch at depth k the FIRST new act lands at index k, whose
stale flag belongs to an act that STARTS an old group (a cursor can
never rest mid-group), so it is always false: harmless. The SECOND new
act inherits the old group's "joined" flag. The test now builds exactly
that (group undone, two plain acts on the branch, undo the second must
stop at the first) and the plant is caught.
