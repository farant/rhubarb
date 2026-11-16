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

## 2026-11-16 - mirrored poses, skeleton sheets, the goblin (plastica motus T5)

`<positura speculum="x">`, skeleton sheets (`<ossa>`, figure
`ossa="name"`), `plastica_figura_legere_cum_fonte`, the `plagula`
error field, `plastica_legere_arboris` in plastica. First shared
skeleton `plastica/ossa/bipes.stml`; first figure on it
`plastica/figurae/goblin.stml` (idle, walk from turns alone).

- **A figure with a skeleton is built by merging two STML trees,
  not two texts.** The skeleton's joint elements are copied, the
  figure's solids are hung inside them (the figure's own nodes, not
  copies), and plastica reads the merged tree through the new
  `plastica_legere_arboris`. Text merging would have lost the line
  numbers.
- **Which sheet an error is in:** copied skeleton nodes carry their
  line with bit 30 set (`LINEA_OSSIUM`); plastica hands lines back
  untouched, so a refusal on such a line is the skeleton's. One
  plastica message prints a line number in its text (duplicate joint
  title, "first on line N"), so duplicate titles in a skeleton are
  caught before plastica sees them; a test checks the flag never
  leaks into a message.
- **Mirror = conjugation by the x reflection.** For a turn written
  as x, then y, then z degrees the mirrored turn is (x, -y, -z)
  exactly, for any rest pose, as long as the pair's rest poses are
  themselves mirror images. That condition is checked (locus,
  rest turn, parent) and refused otherwise, because a mirrored pose
  on an asymmetric pair is silently wrong.
- **`ex` and `speculum` copy values when read.** A skeleton pose
  derived from another does not follow a figure's later replacement
  of the original. A figure that replaces `contactus_sin` and wants
  the mirror to follow must also write `contactus_dex`.
- **After a figure replaces a pose, every cycle's 180-degree check
  runs again,** the figure's own cycles too: a cycle written above
  the replacement saw the old pose.
- **Root motion scale** = figure root height / skeleton root height,
  applied when a skeleton pose is read, to all three components.
  A figure's own poses are not scaled.
- **Found by plants (119 on the library, all red in the end):**
  a pair cache nothing could observe (removed); a scratch slot for
  the cycle being read (removed: a refused cycle refuses the figure
  anyway); skipping motion elements when merging (plastica passes
  them over already); a root-joint condition on the scale factor
  (motus is refused elsewhere on other joints); an early seed check
  in `plastica_legere_seminis` duplicated by the tree reader. Weak
  tests strengthened: pairs with equal-length titles and a shared
  prefix, a nested pair, an unpaired joint off the centre plane,
  replacing at the pose and cycle limits, loose solids at a
  skeleton figure's root.
- **Strip cells are 160 wide now** (120 tall as before; a resting
  figure is still at most 120 wide), so wide poses and lunges have
  20 pixels each side. A plant showed the three-quarter row's tests
  could not see a wrong width handed to the dither (the paper colour
  is not pure white, so holes counted as figure); a dark-pixel check
  now pins it.
- **Authoring the goblin:** limbs hang down from their pivot, so a
  NEGATIVE x turn swings a limb forward and a positive one bends the
  knee back. The first walk sank 0.12 units: the rear foot's toe dug
  in because foot pitch is the SUM of hip, knee and ankle turns.
  Fixed by ankle turns that cancel most of the leg's pitch and a
  shorter foot; now -0.03 .. 0.01. Foot targets (T6) are the real
  answer. Parts overlap 0.02 at each joint; no gaps seen at these
  angles. In the world the red tunic and pale head read well against
  the grass.

## 2026-11-16 - T6: limbs and foot targets

- **New module `plastica_membrum`**: the two-bone solve on its own,
  pure numbers (lengths, target ahead and below, bend direction ->
  two angles in degrees). Root angle negative = forward, knee
  positive = foot back, matching how the sheets already turn limbs.
  Refuses out of reach, folded too tight, target on the root, and
  non-finite input; margin `PLASTICA_MEMBRUM_MARGO` 1e-4.
- **Sheet:** `<membrum titulus radix medium finis flexus>` and, in a
  pose, `<meta membrum locus [inclinatio]>`. Targets are solved
  after every `<art>` of the pose, so a pelvis move written after
  the target still counts. A limb must hang straight down at rest.
  `ex` inherits targets and solves again; an `<art>` on the limb's
  root or middle drops the inherited target; a turn on the end joint
  drops an inherited `inclinatio` only. `speculum` moves a target to
  the partner limb with x negated.
- **`inclinatio`** (not in the header): without it the foot turns
  with the shin, and its pitch is the sum of hip and knee. With it
  the end joint keeps its rest orientation, pitched by the given
  angle (positive = toes down). Every foot target in bipes uses it.
- **Skeleton targets are NOT scaled by root height** (the approved
  header said they would be). The goblin's legs are shorter in
  proportion than the skeleton's, and its feet came out of reach.
  Rule now: offset of the target from the limb's REST END in the
  skeleton, times figure limb length / skeleton limb length, added
  to the figure's rest end. A foot on the ground in the skeleton is
  on the ground in every figure. To know the skeleton's rest ends
  the skeleton is built alone as a Statua with one dummy solid
  (`_quietem_ossium_struere`); the root-height factor reads it too.
- **Foot slide** (`plastica_metiri_lapsum`, plastica_specimen): per
  sample the lowest limb end is taken as planted; while the same end
  stays lowest, its motion is compared with the motion the ground
  demands (speed x time, backward). The tool prints it for cycles
  titled `walk` on figures with limbs, at 4 units/s x figure height
  / 2 (the world sizes every character to 2 units).
- **Limit of the number:** when the heel lifts before the toe, the
  ankle pivot moves although the toe is planted, and that counts as
  slide. The goblin's 0.17 is partly this. Measuring at the lowest
  vertex of the foot solid would be truer.
- **Goblin walk is its own cycle, 0.29 s** (skeleton: 0.44 s). At
  the world's speed short legs must step faster or slide; the stride
  a leg can reach sets the cycle time. Speed per creature is the
  combat spec's business.
- **Plants: 135 on the library, 5 by hand on the tool, all red in
  the end.** Weak tests found: no joint above the pelvis was ever
  moved, so walking up the parent chain was unobserved (fixture with
  two moved joints above the limb); an inherited `inclinatio` with a
  written foot turn (pose `i`); the tool's height scaling (smoke
  fixture: a standing figure 1 tall, walk 0.5 s, slide exactly
  1.00). Kept though green, as explicit guards: `finium == 0` and
  `!piscina` in `plastica_metiri_lapsum` (the allocator refuses both
  today, by its own rules). Not planted: the euler clamps in
  `_gradus_ex_rotatione`, the length cap in `_recusare_duo`.
