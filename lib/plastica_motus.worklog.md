# plastica_motus worklog

## 2026-11-14 - poses and plain cycles (plastica motus T2)

Three passes: `plastica_legere_seminis` builds the body (nodes, joint
table, rest pose); the sheet is parsed a second time for `<positura>`
and `<cyclus>`; baking turns them into `Statua` channels. The new
`Statua` is a copy of the body's value that borrows its tables; only
the cycles are new, and `statua_finire` is run again on the copy.

- **A pose stores degrees, not quaternions.** The 180-degree rule
  compares the written numbers per axis between neighbouring keys, so
  the degrees have to survive until the cycle is read. The quaternion
  is `rest * (Rz * Ry * Rx)`: the written turn is applied in the
  joint's own space, x then y then z (same order as plastica's
  `rotatio`), then the rest pose.
- **The loop's closing key is compared modulo 360.** Keys at 0, 120,
  240 close into a full turn; without this a spin could not be
  written. Only exactly 180 (mod 360) is refused there.
- **No sign-fixing of baked quaternions.** Written first, removed:
  `stereometria_quaternio_slerp` already flips to the short way, and
  for a single-axis turn under 180 degrees the freshly computed
  quaternions have a positive dot product anyway. No test could see
  the step.
- **Every cycle ends with a key at `duratio`:** the first pose again
  for a loop, the last pose held otherwise (skipped when the last
  written key already sits at `duratio`). `Statua` takes the cycle's
  length from the keys, so without it a non-looping cycle would be
  shorter than written.
- **`quies`** is pose 0, built before the sheet is read; a sheet
  that writes it is refused with its own message, not as a duplicate.
- Plants: 80 red. One green by nature: dropping the `n >= numerus`
  guard in `_numeri` still refuses (the count is wrong at the end)
  but writes past the caller's array first. The guard stays; a test
  cannot see an overflow of that size.
- A plant showed the test for "a pose holds only `<art>`" was
  matching on `<os>` alone, which the next refusal also printed.
  The fragment now pins the whole message.
