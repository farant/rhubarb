# Eventus — the input vocabulary (brainstorm interview)

*2026-10-01. An interview with Fran about terminal-planning modules/003
(input vocabulary): one lossless event model for keys, mouse, paste and
focus. It moves out of `fenestra.h` and is shared by fenestra (native
windows), the terminal (over tessera) and later the emulator; the ludus
dispensator consumes it.*

**Why now:** widgets (buttons, text fields, text areas, drag and drop) are
the next step toward the design-vocabulary research (features/021).

**Found while preparing:**
- ludus's event vocabulary (`EVENTUS_*`, `Eventus`) lives in `fenestra.h`,
  and `dispensator.h` includes it, so every ludus app links Cocoa today.
  This is the same header problem tessellatio T4a fixed for the rasterizer.
- fenestra's `Eventus.datum.clavis` carries ONE byte of typed text
  (`character typus`).
- tessera's `TesseraEventum` is deliberately lossy: Ctrl+I equals Tab,
  `\r` and `\n` merge, and there are no releases.
- ludus already derives hover (`super`), focus (captured/lost/requested,
  a focus stack) and double-click, and has actions (`actio`) and
  targeting (`destinatio`).
- `componens.h` reserves the widget kinds `PARTES_BOTTONE`, `CAMPUS`,
  `OPTIO`, `INDEX`, `ITEM`, `DIALOGUS`. None is implemented.

---

## Round 1 — the shape

**Q1. Key vs text.** Typing produces two different things: a physical key
press, and text (which may come from an IME, dead keys like ´+e→é, or a
paste). Browsers and Cocoa model them as SEPARATE events (keydown vs
input/insertText); Ghostty bundles text into the key event. Which shape?
- Separate events: a key event (physical code, modifiers,
  press/repeat/release) and a distinct text event (UTF-8, from typing,
  IME commit, or dead-key composition); paste could be a text event with
  a 'pasted' origin
- Bundled (Ghostty)
- Key only, text derived

**Fran: Separate events.**

**Q2. Mouse units.** fenestra reports pixels; terminals report cells (SGR);
some terminals can report pixels via ?1016, but a terminal without it
silently sends cells, and with zero queries we can't tell which. What
unit should the mouse position carry?
- Our pixels via Modulus (terminal: the cell centre via the cell metric;
  ?1016 unused)
- Both cell and pixel fields
- Cells everywhere

**Fran: Our pixels via Modulus.**

**Q3. Replay.** fenestra events carry a timestamp ("time is data in the
event") and ludus replays sessions from STML. Should replay become a
CROSS-TARGET goal (a terminal recording replays in a fenestra window and
vice versa, producing the same app states)?
- Yes, cross-target: the vocabulary is the recording format
- Same-target only
- Not a priority now

**Fran: Yes, cross-target.**

**Q4. Fidelity.** Terminals often can't report what windows can: key
releases, Ctrl+I vs Tab, left vs right Shift, hover without ?1003. How
should the vocabulary expose what a source can't tell you?
- Per-source capabilities: a small struct published once; widgets adapt
  at setup
- Per-event fidelity bits
- Lowest common denominator

**Fran: Per-source capabilities.**

## Round 2 — semantics and recording

**Q5. Commands.** Shortcuts differ by platform and target (Cmd+C in a Mac
window, Ctrl+C in a terminal, Ctrl+Shift+C in many terminals). ludus
has actions (`actio`) and targeting (`destinatio`). Where should copy,
undo, focus-next and select-word live?
- A keymap → ludus actiones
- Command events in the vocabulary
- Widgets interpret keys

**Fran:** "I'm not too worried about menu / keyboard shortcut stuff for
now. Long term I think we will have ways to automatically/manually
register commands as actions you can take and there will be some kind of
universal command palette with fuzzy search kind of like VS Code. But we
can add platform target stuff as well."

**Q6. Recording pointer events.** A click recorded in fenestra at pixel
(63, 40) replays in a terminal as cell (10, 5). Near a widget's edge it
can land on the neighbour, and app state diverges. What should a
recording store?
- Raw + resolved target: the raw position AND destinatio's result (id +
  local offset). Replay can be raw (tests hit-testing) or semantic
  (robust across targets and layouts), and any divergence between the two
  is itself a finding
- Semantic target only
- Raw positions only

**Fran: Raw + resolved target.**

**Q7. Drop.** Dropping files from Finder: a native window gets a real drop
(paths + position); a terminal "types" the paths inside a bracketed
paste. Should there be a DROP event, and may the terminal source PROMOTE a
paste into a drop?
- Drop event; the terminal promotes a paste made entirely of existing
  file paths (stat'd, shell escaping undone) into a drop at the last
  mouse position; capabilities flag it as a "heuristic drop"
- Drop event; no promotion
- No drop event yet

**Fran: Drop event; the terminal promotes.**

**Q8. Name.**
- Keep `Eventus` in its own header (include/eventus.h, extended
  losslessly; minimal churn for ludus)
- `initus` (new)
- `sensus`

**Fran: Keep Eventus, in its own header.**

## Round 3 — the key event and what counts as raw

**Q9. Key identity.** With text split out, what IDENTIFIES a key? On
AZERTY the key in the US "Q" position types "a". Native windows know both
the position and the character; terminals (without kitty) send only the
character. Shortcuts are usually meant by character, games and
vim-style navigation by position.
- Both: code (physical, W3C-style; may be unknown in a terminal) + the
  logical key (the unshifted character, always present)
- Logical only
- Physical only (Ghostty)

**Fran:** "Both, code + logical key. Someday, if possible, I imagine
we'll try to have a platform layer that lets us know what keyboard is
being used."

**Q10. Motion volume.** ?1003 can flood an app. Where is motion thinned,
given recordings must replay deterministically?
- Coalesce at the source per read (clicks and button changes never
  merged; the recording holds exactly what the app saw)
- Deliver all; the dispensator coalesces
- Opt-in per app

**Fran: Coalesce at the source per read.**

**Q11. IME preedit.** A native window can show the in-progress composition
inline; a terminal handles IME itself and sends only the commit. Model
composition?
- Yes, as a native-only capability: text events have "composing"
  (preedit + cursor) and "commit" kinds; the terminal capability says
  commit-only
- Commit only for now

**Fran: Yes, as a native-only capability.**

**Q12. Derivation.** macOS can report clickCount = 2; terminals never do.
ludus already derives hover and focus. What's the rule?
- Sources emit raw events only; ludus derives everything (double-click
  threshold, drag threshold in our pixels) from raw events + timestamps;
  recordings hold raw events only, so derivation is replayed and tested
  identically everywhere
- Native derivations allowed
- Raw only, with OS settings as data

**Fran: Sources emit raw events only.**

## Round 4 — paths, scroll, pointers, payloads

**Q13. Brush paths.** Coalescing thins hover traffic, but pictor's brush
needs every point. The web solved this with `getCoalescedEvents()`. Should
our coalesced motion event carry the skipped samples?
- Yes: one event with a small array of intermediate positions +
  timestamps; forms ignore it, pictor iterates it
- No, the last position only
- Per-app opt-out of coalescing

**Fran: Yes, carry the samples.**

**Q14. Scroll.** Trackpads give smooth fractional deltas (fenestra uses
f32), wheels give notches, terminals give one event per notch. The house
prefers integer maths.
- Integer our-pixels + a kind: precise (trackpad) or notched (wheel or
  terminal, where one notch = a source-declared number of pixels, e.g. a
  cell height); the source accumulates sub-pixel remainders; no floats
- Lines + pixels (DOM deltaMode)
- Keep f32 deltas

**Fran: Integer our-pixels + a kind.**

**Q15. Pointers.** Pen pressure/tilt (pictor), trackpad gestures, multitouch.
Following "get the API right first", what should be reserved now?
- A pointer id (0 = mouse), a kind (mouse/pen/touch) and pressure
  (integer 0..1000, unknown for mice and terminals); tilt and gestures
  later
- The full pointer model now
- Mouse only, extend later

**Fran: Pointer id, kind and pressure.**

**Q16. Payloads.** Pastes can be 64 KiB; tessera hands out views valid
until the next read. Recordings need the bytes to outlive that. Who owns
payloads (paste text, drop paths, composed text)?
- Views; the recorder (and any consumer that keeps data) copies
  explicitly; the lifetime rule is documented once for all sources
- A per-frame arena
- Interned

**Fran: Views; the recorder copies.**

## Round 5 — capabilities, migration, scope, conformance

**Q17. Pushing kitty.** If tessera pushes the kitty keyboard protocol, a
supporting terminal upgrades and a non-supporting one ignores it. We
can't ask which.
- Learn by observation: push the flags; capabilities start at legacy and
  UPGRADE the first time a kitty-format sequence arrives (observed, never
  queried); apps get a "capabilities changed" event
- Don't push kitty yet
- Push, and trust the environment (TERM_PROGRAM)

**Fran: Learn by observation.**

**Q18. Migration** (`character typus`, used by ludus and pictor today).
- Extend, then retire `typus`: eventus.h with the lossless shape;
  fenestra fills the new fields AND the deprecated `typus`; consumers
  move to text events; then `typus` is deleted. Each step green
- A clean break

**Fran: Extend, then retire `typus`.**

**Q19. Scope.** Decode (devices → Eventus) vs encode (Eventus → terminal
bytes, for a future emulator).
- Decode now with an encode-ready API: physical code, unshifted key, and
  modifiers with sides; the encoder and its round-trip test come with
  the emulator arc
- Both directions now

**Fran: Decode now, with an encode-ready API.**

**Q20. Conformance.** A shared table of physical scenarios ("press Shift,
type a, release Shift", "drag A → B", "paste 3 file paths") with the
expected Eventus stream, run against fenestra (injection API) and the
terminal (byte vectors), with differences allowed only where
capabilities say so.
- Yes, as THE acceptance bar; new sources (vitrea, the emulator) must
  pass it
- Per-source tests only

**Fran: Yes, as the acceptance bar.**

## Round 6 — widgets and design

**Q21. States.** Interaction capabilities look like colour depth in 021
("targets differ in what they have, never fork the vocabulary"). No hover
on touch or without ?1003.
- States degrade by capability: hover styling simply never appears, and
  nothing essential may be hover-only (a design invariant the gate
  checks)
- States are target-specific

**Fran: States degrade by capability.**

**Q22. Keyboard.** Every widget fully operable by keyboard alone (focus
traversal, activation, editing)?
- A hard rule, tested: a keyboard-only replay script per widget;
  groundwork for accessibility
- A strong default, not enforced

**Fran: A hard rule, tested.**

**Q23. Where the terminal source lives.** tessera's lossy `TesseraEventum`
serves saltuarius, folium and effigies, and tessera's pin is "grid +
input, forever".
- A separate library over tessera: tessera keeps `TesseraEventum` as a
  projection (saltuarius untouched); a new layer (module 013
  `ludus_tessera`, or an "eventus_tessera" source) reads bytes through
  tessera's reader and produces lossless Eventus
- tessera switches to Eventus

**Fran: A separate library over tessera.**

**Q24. When to request ?1003** (bandwidth, noticeable over ssh).
- When the app declares hover (or ludus sees a hover style); ?1002
  otherwise; capabilities report it
- Always on
- The environment decides (SSH_CONNECTION)

**Fran: When the app declares hover.**

## Round 7 — clipboard, selection, lifecycle

**Q25. Clipboard.** A native window writes the system clipboard. A
terminal app can WRITE with OSC 52 (an output escape; Ghostty, iTerm2,
kitty; Terminal.app ignores it), but READING via OSC 52 would be a query.
- Write via OSC 52; reading = paste: clipboard write is an output
  capability (fenestra native; terminal OSC 52, "maybe"); content
  arrives only as paste events; zero queries holds
- A platform layer (pbcopy/pbpaste)
- Both, chosen by environment

**Fran: Write via OSC 52; reading = paste.**

**Q26. Native selection.** Capturing the mouse takes the terminal's own
selection away (most terminals restore it with Shift+drag).
- Leave Shift+drag to the terminal (documented); widgets select with
  plain drag and copy via OSC 52
- Release the mouse when idle

**Fran: Leave Shift+drag to the terminal.**

**Q27. Lifecycle** (close, resize, suspend/resume, window focus).
- In the vocabulary, and recorded: resize (in our pixels), window focus
  in/out, suspend/resume and close request are ordinary Eventus kinds;
  replay reproduces layout changes and focus dimming exactly
- In the vocabulary, not recorded
- Outside the vocabulary

**Fran: In the vocabulary, and recorded.**

---

## Synthesis (Claude; for the spec that follows)

**The model.**
- `Eventus` moves to its own header, `include/eventus.h` (no Cocoa), and
  becomes lossless.
- Kinds:
  - **key**: code + logical key + press/repeat/release + modifiers with
    sides;
  - **text**: commit or composing (preedit + cursor); origin typed /
    pasted / composed;
  - **pointer**: id, kind, pressure; position in our pixels; buttons;
    coalesced samples;
  - **scroll**: integer our-pixels + precise/notched;
  - **drop**: paths + position; possibly promoted from a paste;
  - **lifecycle**: resize, window focus, suspend/resume, close;
  - **capabilities changed**.
- Every event carries a timestamp.
- Payloads are views. The recorder copies.

**Sources emit raw events only.** ludus derives double-click, drag, hover
and focus from raw events + time.

**Capabilities.** Each source publishes:
- releases;
- the physical code;
- Tab vs Ctrl+I;
- left/right modifiers;
- hover;
- preedit;
- clipboard write;
- heuristic drop;
- the notch size.

They are learned by observation where pushing a mode is possible
(kitty), and never queried.

**Recordings** store the raw event + the resolved target, and replay in
raw or semantic mode across targets.

**Acceptance:** a shared conformance table that every source must pass.

**Widget rules:**
- states degrade by capability (no essential hover-only behaviour);
- every widget is fully keyboard-operable (a tested invariant).

**Terminal source:** a separate library over tessera's reader. tessera's
own lossy event stays as a projection. It requests ?1003 only when hover
is declared, pushes kitty and learns from observation, writes the
clipboard with OSC 52, and leaves Shift+drag to the terminal.

**Deferred:**
- the encoder (emulator arc);
- commands, keymaps and the fuzzy command palette;
- detecting the keyboard layout through a platform layer;
- tilt and gestures.

**Open questions for the spec:**
- the exact W3C code subset for v1;
- double-click and drag thresholds (constants or capabilities);
- the size of the coalesced-sample array (fixed cap?);
- how "capabilities changed" interacts with recordings (record it as an
  event: yes, follows from Q27's logic);
- where the conformance table lives (data, not code, shared by both
  source suites).
