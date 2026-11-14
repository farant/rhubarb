# scaena.c worklog

## 2026-11-13 - T6, scenes

- A plant that loosened the per-field maximum in `_lege_campum`
  stayed GREEN. The test that set the title length to 65 inside a
  valid blob was refused only because every later field was then
  misaligned. A length check needs a blob that is otherwise correct:
  the tests now build views by hand with a field one byte too long,
  write them with `scaena_scribere` (which does not check lengths)
  and require `scaena_legere` to refuse.
- `longus` is a latina.h macro (`long`). The compile error reads
  "'long type-name' is invalid".
- Lengths are unsigned (`i32`): every bound is written as
  `x > mensura - cursor` only after `cursor <= mensura` is known.
- `stml_textus_internus` gives the raw text; whitespace is collapsed
  here (`_textus_contractus`), so prose in a scene file can be
  wrapped and indented freely.

## 2026-11-14 — view blob is 480 x 320

No format change beyond the size: `PIXELA` follows graphis.h. A blob
built at 360 x 240 fails the length check in `scaena_legere` and the
pane shows "cannot be read (rebuild it: bin/scaena condere)". The
magic stays "SCAENA01".
