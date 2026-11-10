# membrum__delectus worklog

## 2026-11-09 - T7 of project-specs/statuae-plan.md

- **What it is.** A dropdown: a button showing the current value that
  opens the house popup menu (`elenchus`) under itself. Written for
  the statuae pane's header (clip, dither) but generic; nothing in it
  knows about models.
- **Same state pattern as `membrum__elenchi`**: the host pane declares
  two transient fields in its manifest and the member reads and
  writes them through the host's writer. `delectus` holds the id of
  the open dropdown (`aspectus="popover"`, so dismissal closes it),
  `delectus_electa` the highlighted option.
- **Different contract from `membrum__elenchi`**: that member runs a
  command from the host's `Praeceptum` table. A dropdown's options
  are data (clip names from a file), so `delectus_eventum_agere`
  returns DELECTUS_ELECTUM with the index and the host writes its own
  field. The host passes the options as an `Elenchus` it builds each
  time; the member stores none.
- One field for "which is open" makes "only one open at a time" a
  consequence, not a rule to enforce.
- **Menu placement.** `elenchus_fines` reflects a menu that does not
  fit below its anchor upward from the anchor point, which here would
  cover the button. `delectus_componere` detects that (the returned
  y is above the anchor) and puts the menu wholly above the button.
- **Host recipe** (for T8): on every event call
  `delectus_eventum_agere` first; DELECTUS_NIHIL means the event is
  the host's. A click on the button while its dropdown is open
  arrives as "click outside" and returns DELECTUS_CLAUSUM, so the
  host must not reopen on that same event.
- Events that are neither mouse nor key (focus) return DELECTUS_NIHIL
  even while open.
- The button draws its own label left-aligned and a 5-3-1 pixel
  triangle; the 6x8 font has no reliable down-arrow glyph.
- Glossary: form `bottonem` added to the existing `botton` entry.
- Plants: Enter does not select -> "claves"; menu allowed to cover
  the button -> "locus"; every id reported open -> "status".

## 2026-11-10 - j / k with real key events (statuae T9)

`delectus_eventum_agere` now turns j / k into the arrow keys before
calling `elenchus_clavem_agere`: real key events carry the logical key
uppercase ('J') and the typed letter in `runa`, and the menu compares
`clavis` with lowercase. Found in vicus (the highlight did not move);
the T7 suite had hand-built events with a lowercase `clavis`, which is
not what the window layer sends. The suite now has a real-shaped
event. Lesson: build key events in tests the way `manus_ludus` does.
