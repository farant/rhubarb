# git worklog

## 2026-08-17 — natus: lector repositorii git (lectio sola)

Born same-day as the differre arc that motivated it ("query git
changes from C"). ~1,100 lines, first-compile first-run green
against the REAL rhubarb .git (4 packs, 5,117 loose objects,
HEAD~40 delta chains). Notes worth keeping:

**Self-verifying reads.** A pure reader never needs SHA-1 — until
you make verification the point: git_obiectum_legere recomputes
"genus mensura\0corpus" SHA-1 on EVERY read (loose and
pack-assembled) and compares to the requested sha. This (a) makes
corruption loud, (b) turns every probatio transit into a SHA-1
test against real objects, (c) is why the interior _sha1 isn't
dead code. The poisoned-CRC lesson (imago stack) applied at birth.

**flatura carried the day.** Zlib = 2-byte header check +
flatura_inflare (raw RFC 1951) + adler32 trailer (verified for
loose objects; UNVERIFIABLE for pack entries — compressed length
unknown, trailer unlocatable; the sha verification covers it).
flatura's bit reader stops at the final DEFLATE block, so trailing
pack bytes are never read — measured, this is what makes
per-object windows work.

**Pack windows, not pack loads.** Packs run to 78MB; idx files are
tiny (≤171KB) and load whole at aperire. Objects read via
fseek/fread windows starting 64KB, ×4 growth on
FLATURA_STATUS_INCOMPLETUS. OFS_DELTA recursion by offset within
the pack; REF_DELTA recurses through general lookup (may hop
loose/pack). Depth cap 64.

**Gotchas met:**
- OFS_DELTA negative-offset varint has the +1-per-continuation
  encoding (negativum = ((negativum+1)<<7) | bits) — NOT the plain
  size varint; classic first-implementation trap, written from
  memory of the spec and verified by the deep-history tests.
- idx v2 large-offset table (MSB set in the 32-bit offset) —
  implemented though rhubarb's packs don't need it yet; the format
  demands it for >2GB packs.
- commit parse must skip continuation lines (leading space —
  gpgsig); message = everything after the first empty line.
- Annotated tags peel via "object <sha>" lines (_ad_commissum_
  pellere, cap 8) — refs may point at signa, not commissa.

**Deliberate scope cuts (doors, not gaps):** short-sha resolution
(easy once wanted: idx is sorted — prefix binary search), ^ and
ranges in rev-parse, worktree status, log-walk helpers, index/
staging reads, SHA-256 repos (refused loudly at aperire).

**Test strategy:** no fixture repo — the live repository IS the
fixture (suite runs from root), and the external oracle is
byte-comparison against the worktree file (latina.h — chosen
because it never changes). Deep-history reads exercise packs and
deltas naturally.

## 2026-08-17 (later) — sha breve + differre goes native

The short-sha door lasted exactly one invocation: the FIRST real
differre -git call pasted `2147d9d` (humans paste short shas; the
subprocess wrapper had hidden this by letting git resolve them).
Implemented as last-resort fallback in ref_resolvere (refs beat
prefixes): pack idx fanout-bucket scan + loose two-char-dir scan,
cross-source dedup (same object loose AND packed is one candidate,
not ambiguity), unique-or-refuse, minimum 4 hex.

differre gained `-git <via> [ref_vetus] [ref_novum]` (native
library sides, worktree default for the new side, absent-at-ref =
empty side); differre_git.sh is now a two-line alias with the same
face — the subprocess guts lived one day, as planned.

## 2026-08-18 — git_arbores_differre (differentia arborum)

The recursive tree-vs-tree diff, the sigillum-screen pattern one
level up: entry shas are compared BEFORE any content is read, so
an unchanged subtree costs one string compare no matter how many
thousand files live under it. Only changed paths ever open blobs.

Design decisions:
- Hash-join by entry name per level (tabula_dispersa, ordinal
  cells — Xar is segmented, no cross-element arithmetic), NOT a
  sorted merge-join: git's tree ordering treats directories as
  name+"/" ("foo.bar" sorts before directory "foo"), and a
  merge-join replicating that quirk is a bug farm. The join is
  order-blind; determinism comes from one xar_ordinare
  (chorda_comparare) over the finished list.
- Mode-only change (same sha, 100644<->100755) = MUTATA row with
  equal shas — honest, and downstream unit-diff naturally reports
  zero pairs.
- Type flip (blob<->tree, same name) = REMOTA of the whole old
  side + ADDITA of the whole new side. No "modified" lie.
- Gitlinks (mode 160000, submodules) fall out as blob-shaped rows;
  shas compare fine, content reads would refuse (not a MASSA).
  Rhubarb has no submodules; noted, not handled specially.

Oracle: frozen history. 4aafde9 (this library's own birth commit)
vs parent d4a89dd, expectations transcribed from
`git show --name-status`: 1 M + 4 A in chorda_comparare order,
plus tree-vs-itself = empty and the reversed direction flipping
ADDITA<->REMOTA. First run green.
