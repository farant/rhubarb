# derivare.worklog.md

## 2026-09-04 — natus (ludus T4)

Double-click by event timestamps (interval + distance), reset after a
double so a triple is not a second double. Pure: no clock, no state
beyond the Derivator the caller owns. This is what retires the
`tempus_nunc()` call inside widget.c's manager (unreplayable). Hover
and focus derivation live in the dispensator (T10) — they need the
targeting result. `/* <purus/> */` above `derivare` is the L2 anchor
for the examen codex when it lands (annotationes chunk C).

Second latina.h collision of the day: a local named `duplex` —
`#define duplex double`. Now `est_geminus`. Born from it:
`tools/latina_custos.sh <files>` — refuses any DECLARED identifier
(after a type: field, local, parameter) that is a latina.h macro name;
keywords used as keywords pass. Verified against a planted fault
(`b32 duplex; i32 nomen;`). Gate order in every ludus task:
latina_custos → formator -scribere → -vitia 0 → compile_tests →
examen → commit.

## 2026-10-14 - DUPLEX only for presses of the SAME button

The calendar's context menu (M2c) opens with its top-left corner at the
pointer, so a quick right-click followed by a left-click on the first
item used to pair into a DUPLEX: the left press ran the item and closed
the menu, then the DUPLEX landed on whatever was underneath (on a month
cell: open the day). Pairing now also requires
`ev->datum.mus.botton == d->botton`. No new field: `d->botton` (the
drag's button) is written only on a press and only in
_tractum_derivare, which runs AFTER the pairing check - so at check
time it still holds the PREVIOUS press's button. (Proposed to Fran as a
new header field first; the existing field turned out to carry exactly
this.) Tests: right then left -> no DUPLEX; right twice -> DUPLEX with
MUS_DEXTER. Existing tests send botton 0 on every press, so they still
pair.
