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

## 2026-11-16 - figures into the volume; the slime (plastica motus T4)

`bin/plastica condere` builds `plastica/figurae/*.stml` as characters
after the props (`-figurae DIR`; default directory only when
`-fontes` is not given, and then a missing or empty one is not an
error). First figure: `plastica/figurae/limax.stml`, title `slime`,
cycles idle, walk, attack, hurt, die.

- **A `<figura>` sheet in the props directory is refused.** After T1
  `plastica_legere` accepts either root, so without the check a
  figure dropped among the props would be stored as a prop.
- **Duplicate titles are checked across props and figures of one
  run** (one shared title table); the "no source" report still looks
  only at the props part of it. Bought characters never have a
  source, so figures get no "no source" lines.
- **Authoring the slime, what the strip and its numbers caught:**
  - Tilting a wide body about a pivot on the ground pushes its rim
    under the ground (`infimum -0.20` in the first attack). Fixed by
    lifting the pose a little; the number came back to -0.01.
  - A base disc ("puddle") read as a hat brim from three-quarter
    view and floated when the slime hopped. Removed.
  - Squash by scale about a ground pivot works as hoped: one joint,
    five cycles, no limbs.
- **Seen in the world (headless):** the slime stands and hops, and
  scale keys show there (spec AUDIENDA 4 answered). A green slime on
  the clearing's grass was nearly invisible; blue, magenta and
  orange were tried side by side and blue kept. Colour against the
  ground matters more than shape at this size.
- **Strip limit found:** a figure near 3 units wide fills the 120
  pixel cell, so a wide squash or a forward lunge is cut off at the
  cell edge (attack frames 3-4). Wider cells would fix it; not done
  here.
- The world still sizes every character to 2 units tall, so the
  slime stands as tall as the mushroom man (combat spec).
