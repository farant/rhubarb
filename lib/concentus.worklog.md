# concentus worklog

## 2026-10-22 - S5b: one shared sound host per process

Why: `auditus` is ONE output per process, and S4's tintinnabulum owned a
mixer + rector + device of its own - the aetherophonum (S5c) could not
open a second one. Fran OK'd moving the device lifecycle into a host.

Model:
- LIVE = a slot occupied in the rector. The rector frees a slot only when
  VOX_FINITA is read (concentus_agere is the ONLY reader of messages) or
  when a STARTING command is refused (concentus_iubere frees it - else a
  slot stays occupied forever with no voice, and the host never goes
  quiet). Hence the creation rule nuntii >= II x voces (spec, auditus.h):
  a lost message would be a slot never freed. The tintinnabulum's old
  "terminus" fallback for lost messages is gone - the rule replaces it.
- Clients poll `concentus_vox_vivit(ansa)` (rector slot + generation);
  no callbacks.
- Quiet close after CONCENTUS_QUIES (30 s) with no live voice; the quiet
  clock restarts whenever a voice lives (a plant that kept the old clock
  went green until the quies section got "quiet, voice, quiet").
- `concentus_quiescere` = close at the first moment no voice lives (a
  mute: after the fade, not 30 s later); another client's live voice
  keeps the device open. `concentus_aperire` cancels it - the
  tintinnabulum calls it when a new occasion arrives during the fade
  (section tintinnabulum/redit: no close-and-reopen).

Rector gained mixtura_rector_vivit / _dimittere / _occupatae. Test lessons
from plants (5 green at first): a stale-handle check against a FREE slot
proves nothing - test against a slot REUSED by a newer generation; a
latency check with mora < margin cannot see a missing mora (fake device's
mora made settable). 20 plants red in the end.
