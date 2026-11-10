# statuae_repositorium worklog

## 2026-11-09 - T6 of project-specs/statuae-plan.md

- **Shape.** A model is its original file bytes as a volume blob plus
  one act `statua-addita` {titulus, massa, forma}; `statua-remota`
  {titulus} takes it out. The list is a replay of those two genera.
  Nothing else is stored, so nothing can drift.
- **Own acts only** (`volumen_acta_generum_replicae_legere` with this
  volume's replica; unfiltered only when the replica is empty). The
  negotia library reads every replica's acts; not copied here on
  purpose - a synced act could name a blob that has not arrived.
- **Another process's write is visible**: section "seq" opens a second
  handle on the same volume file; `volumen_seq_ultimum` rises on both
  and the second handle's list shows the model. This was an open
  question in the spec (AUDIENDA) and is what lets the pane notice
  `bin/statuae addere` while vicus runs.
- Re-adding the same bytes under the same title writes no act (the
  blob is content-addressed anyway). Removing an unknown title writes
  nothing. Both pinned by `volumen_seq_ultimum` staying put.
- `statuae_addere` joins the caller's transaction if one is open,
  otherwise opens its own around blob + act.
- Titles: 1..64 bytes, no `/`, no control characters. Titles go
  through the JSON writer, so quotes and backslashes survive.
- Functions that need scratch memory but take no arena make a small
  dynamic one and destroy it (addere, removere).
- Plants: removals ignored in the replay -> "removere", "seq";
  identical bytes rewritten -> "substitutio".
