# aemulator_hospes worklog

## 2026-10-06 — B4a: built

**i32 is unsigned - the queue clip underflowed.** `in_caudam` clipped
with `n > limes - h->mensura`. Replies may fill the queue past the
user-input limit (into the reserve); then `limes - mensura` wraps to
~4e9, the clip never fires, and a user write runs past the ring. examen
flagged the neighbouring `cfg->x < ZEPHYRUM` checks as "comparatio
vana" (always false for unsigned) - reading those led to the clip. Fix:
`mensura >= limes` -> 0 first. Test: after a reply uses the reserve,
`scribere` returns 0 (plant P3 = the old code, caught).

**finitus needs BOTH ends.** `aemulator_hospes_exitus` may reap the
child early (a front end asking "did it exit?"); the pulse still
reports `finitus` only after EOF, so output a dead child left in the
pty is drawn. Test with the obstinate child: dead + 6 bytes pending,
cap 4 -> first pulse not finished, second finished, text complete.

**The cap is enforced twice (plant P4 survives by construction).**
Removing `p.lecti < per_pulsum` from the loop changes nothing: each
read asks for at most `per_pulsum - lecti`, and at 0 the vtable
returns 0. Kept the explicit condition (it does not rely on every
bridge's zero-capacity behaviour); P16 (read a full buffer regardless)
proves the cap test measures the cap.

**The "obstinate" child.** A Pseudoterminale defined in the test:
write quota, endless/cyclic output, EOF and death flags, max bytes per
read, count of reads that were asked to wait. Every queue and loop
property became an exact assertion - the vtable seam from module 008
doing its job.

**Memoriae test byte counts.** Two lengths in my own test were wrong
(a 9-byte literal passed as 10 - the NUL; a 7-byte script as 6 - the
OSC terminator cut). Count literal bytes, never eyeball them.
