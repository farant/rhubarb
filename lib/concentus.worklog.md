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

## 2026-10-26 - tenere: a client with future notes holds the output (parcum ...ZQNDJ6)

Bug: after CONCENTUS_QUIES (30 s) with no occupied voice and no reverb tail, concentus_agere
closed the output, and concentus_iubere never reopens - every later command was dropped.
So a part rendered ALONE that enters after 30 s of tacet (auscultatio_partem -> recensio's
per-part table, -auscultare) measured silent although the full mix had it (clarinet in the
J8Q9H showcase, rms 0 vs peak 0.47 in the mix), and any song with a 30 s general rest went
quiet from there.

Fix: concentus_tenere(c, VERUM/FALSUM) - a counter of holders; while > 0 the quiet timer
does not run (quies_nota reset, so the 30 s count starts at RELEASE, not at the earlier
silence). Mute (quiescere) still closes regardless - checked before the hold. The counter
never goes below 0. The cantor holds from _ponere (incipere / quaerere) until the region
ends without looping or cantor_sistere (cantor.worklog.md). The quiet close stays a
feature for the live device (vicus).

Own bug while doing it: concentus_creare sets fields one by one (no memset) and I forgot
the new 'tenentes' - the cantor suite showed the SAME concentus address in every section
(each section's pool reuses memory) with 14 leftover holds; run alone it passed. Test now
creates a concentus on dirty memory and expects it to close after 30 s. Careful: the
first version used piscina_vacare to "reset" a 0xFF-filled pool - vacare ZEROES the
buffers, so the memory was clean and a plant removing the init stayed green.
piscina_notare / fill 0xFF / piscina_reficere is the path that leaves old bytes in place
(by design - piscina.h warns about exactly this), and the credo section pools reuse
memory the same way.

Plants 9/9 red after three rounds: the dirty-memory test (above); a plant whose anchor
ended inside a comment (silva.planta refuses - anchor the code line before the comment);
and the "mute closes even when held" test was broken by my own later insertion - the new
release-timer test before it left the output closed with no holds, so quiescere on a
closed concentus passed trivially. The mute test now opens and holds first.
