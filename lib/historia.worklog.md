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

## 2026-10-05 — namespaces: several documents in one volume (insula-rami-plan R2)

Fran chose ONE host volume for all of schirmata's tabs, so several
histories must share one act log. `historia_creare/_aperire` take a
`spatium` (namespace) right after the volume: with "s1" the act genus
becomes `s1/<genus>`, the markers `s1/ramus` and `s1/coniunctio`, the
checkpoint keys `s1/checkpoint/<seq>` (the slash keeps `a` and `ab`
apart); each history reads only its own. Empty = the old bare names -
the H0 golden is byte-identical (root 201/201 with "" at every existing
call). pictor_documentum and scriba_documentum gained the same
parameter and prefix their manifest (`s1/documentum`).

Test (probatio_historia XIII): two histories, interleaved acts,
independent undo, a branch in one leaves the other's acts alive,
reopen both from the volume, every checkpoint key prefixed; scriba XI:
two documents reopen to their own sheets, an unknown namespace opens
nothing. Plants: bare markers - **caught only on REOPENING** (both live
objects stayed right; the replay of b saw a's `<ramus>` and dropped its
own act) - the incremental path and the replay can disagree, so the
test must read back from the volume; bare checkpoint keys; bare act
genus.

Slip on the way: my helper's parameter was named `nomen` - the latina
macro for `typedef` (compile error, renamed `titulus`).

## 2026-10-05 — the undo position is persisted (insula-rami-plan R5)

Found in R4: undo moved an in-memory cursor only; reopening went to the
log's end, so undo-then-quit brought undone text back (pictor and
scriba). Fran: persist it as an act. `historia_revocare/_reficere` now
append `<cursor ad="seq"/>` (genus `cursor`, namespaced); `aperire`
scans the log and reopens at the LAST marker unless a client act, a
`<ramus>` or a `<coniunctio>` followed it (then the end, as before);
the marker must name a live act (or 0). Redo works after reopening -
the acts after the cursor are still live.

**The H0 golden changed on purpose** (its session undoes three times,
then once more after the branch, then redoes). Diff inspected: five
`cursor` markers added, every later seq shifted by 3 (cursor/finis
12 -> 15, `checkpoint/12` -> `checkpoint/15`), NO sigillum changed, the
checkpoint's content hash identical, the reopened state the same.
Re-pinned.

Tests (XIV): undo + reopen lands on the undone position; redo after
reopening; reopen after redo = end; undo to 0 + reopen = empty; a new
act after undo makes the marker stale; a joined group undone + reopened.
scriba's two-mount test now asserts the undone text STAYS undone after
reopening. Plants: no marker on undo; stale marker honoured; no marker
on redo - all caught (the stale one first written as `FALSUM && …` in
an `||` chain: -Wlogical-op-parentheses, redone).
