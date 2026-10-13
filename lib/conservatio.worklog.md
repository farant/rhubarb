# conservatio worklog

## 2026-10-10 — S3: the persistence system (status-plan S3)

First time widget state survives a restart (census XII: no island ever
reached disk before). One volume per app (Fran: "all the state of the
different kinds ends up in a single volumen for an app that you run") -
already true for documents and vicus's layout; widget state joins as
plagula "status/<clavis>" (clavis = branch id; standalone root = genus).

Mechanism: the system owns the repository's `InsulaActarius` (called
after every successful gate write; nothing used it before). On each
write, every registered branch whose PERSISTED part changed is saved.
So transient writes cost nothing, kinetic fields save once per quiet
flush, and a crash loses nothing. No dispatcher hook needed.

Format `<status versio="1" genus="..."><durabilis .../><ephemera .../>
</status>` keeps each field's island without the manifest knowing it
(the split may collapse after S5; the format then simply has one
child). Restore = migration: unknown or non-persistent fields dropped,
missing ones keep the initial value, the write goes through the gate
(canon judges) - refused = state skipped with a message, mount never
fails. Unknown version = skipped. Restore happens after initiation,
BEFORE domini (anonymous, like initiation) and before the branch is
registered (its own restore write does not re-save). The return value
counts CHANGED fields only (the id restores to itself).

Bugs found while building, both worth remembering:
- The restore parse lived in the scratch arena; restoring the durable
  island fires the actarius, which resets that arena -> the parsed
  tree vanished before the ephemera island. Restore now parses in the
  mount-time arena. (Symptom: hand-written saved state restored fine,
  real round trips did not.)
- vicus: `vicus_aperire` creates the repository AND mounts panes in
  one call, so the system cannot be created from outside in between.
  The first pictor pane creates it lazily from the `repo` its mount
  callback receives; later panes share it (ContextusPictoris).
  probatio_vicus_conservatio caught this - the library test could not.

Plant lesson (P3): planting "do not update the cached last-saved state"
by removing only the assignment SURVIVED - the line before resets the
branch arena and the new copy lands at the same address the stale
chorda points at; equal lengths -> compares equal. The arena header's
"silent stale read" in action. The sharper plant (no reset, no copy)
is caught. Also P5: removing only the null guard on `versio` survives
because the comparison after it still rejects - plant the comparison.

Classification fixes S3 forced (manifests): superficies_* (window size,
an observation re-measured every run) and pictor latitudo/altitudo
(copies re-derived from the drawing at mount) -> transiens. Restoring
either could only be stale.

Wiring: pictor_montare(..., Conservatio*) - NIHIL = no persistence
(tests). Standalone pictor creates one in pictor_applicatio_aedificare;
vicus as above. Scriba not wired until S6 (then the vicus-level holder
moves out of ContextusPictoris to app level).
