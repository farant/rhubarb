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

## 2026-11-10 - kinds: character or place (mundus-plan T8)

An entry now has a kind (`StatuaeGenus`): STATUA (a character, shown
by `$models` and offered as a persona in `$world`) or LOCUS (a place
`$world` walks in). The "added" act carries `"genus":"locus"` for a
place and NOTHING for a character, so a character's act is byte for
byte what it was before, and every act written before this change
reads as a character.

`statuae_enumerare` and `statuae_addere` keep their meaning
(characters), so the `$models` pane needed no change and never lists
a place. Titles are unique across BOTH kinds: adding a title that
exists in the other kind is refused before anything is written
(otherwise a re-add would silently turn a character into a place).
`statuae_removere` removes either kind.

Tool: `statuae addere ... -locus`, `statuae enumerare ... -loci`;
smoke script extended (it adds the real level fixture as a place).

Caveat: a vicus built BEFORE this change reads a place's act as a
character (it ignores the unknown field) and would show the level in
`$models`. No such build has statuae (it is not in main yet).

Plants, three red: listing without the kind filter; kind not written
to the act; a title from the other kind accepted.

## 2026-11-12 - plastica T6: the third kind

`STATUAE_GENUS_RES` ("genus":"res" in the act): a plastica prop.
Titles are unique across all three kinds. `statuae_enumerare` still
lists characters only, so the `$world` character chooser never sees
a prop. Order of arrival (plastica spec 14d): an older vicus reads
an unknown kind as a character, so the vicus that knows `res` must
run before the first prop is written to a live volume.
