# admonitiones worklog

## 2026-10-15 - T4a: generic notifications (colloquia-plan)

Header approved by Fran. A sorted index (newest first, insertion by
time), duplicate ids refused, the oldest dropped beyond `maximum` (an
entry older than everything in a full index is refused outright).
Every string is COPIED into the index's pool on `addere`: sources read
from transient pools (vicus reads the event log into a per-tick scratch
pool), so views would dangle - the `copia` section mutates the source
buffer after adding.

`admonitio_ex_eventu` is the only Claude-specific part: Notification
(permission_prompt, agent_needs_input, elicitation*) and StopFailure ->
ATTENTIO, Stop -> NUNTIUS with the first line of the last reply; other
events (PreToolUse, idle_prompt, ...) are not notifications. id =
"colloquia:<session>:<time ms>"; action "panellus" with argument
"<pane> <tmux socket>" for T4c.

Same commit: `praeconium_legere_ab` (approved addition) - read from an
offset to the end, consume only up to the last newline (a line being
written stays for the next tick), restart at 0 when the file is shorter
than the offset (deleted/rotated), offset 0 when absent. A red-phase
stub run crashed on an empty Xar and skipped `purgare`, leaving a temp
dir - the count is now CREDO_NECESSE before the element is read.

Plants red: sort reversed, duplicates admitted, no copy, no eviction,
Stop not cut to the first line, permission not recognised; legere_ab:
half line read, rotation not detected.

## 2026-10-22 - Admonitio.genus (sonus S4)

`Admonitio` gained `genus`: the occasion as a cantuale tag ("permissio"
for permission/input/elicitation, "finis" for Stop, "vitium" for
StopFailure). `gradus` could not carry it: ATTENTIO covers both a
permission prompt and a StopFailure, which want different jingles. The
names are the cantuale tags on purpose, so the vicus sound system maps
notification -> jingle with no table. `admonitiones_addere` copies it
like every other chorda (the copy test caught the missing copy first).
