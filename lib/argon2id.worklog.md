# argon2id worklog

## 2026-10-17 - born (hospitium v3 T5)

- **Lanes run one after another, and that is exact:** inside a slice a
  block may reference another lane only in FINISHED slices (pass 0) or
  outside the current segment (later passes), so computing (pass,
  slice, lane, index) in that order gives the same bytes as parallel
  lanes. The RFC vector (p=4) proves it.
- **Resumable state** = (transitus, scissura, lana, index) plus the
  current Argon2i address block and its input block (counter in word
  6). The address block is regenerated at the segment's first index and
  every 128 indices; for pass 0 slice 0 the first index is 2 (columns
  0-1 come from H').
- **Reference index** follows the RFC/reference `index_alpha` literally
  (area size, J1^2 >> 32 mapping, start position for later passes).
  The `- (index == 0 ? 1 : 0)` term for other lanes is easy to miss.
- **Version 0x13:** passes after the first XOR the new block into the
  old one (`cum_xor`).
- Memory is one piscina allocation of m' KiB; use a piscina per hash and
  destroy it after `argon2id_digestum` (which also zeroes it).
- RFC-text trap: the 5.3 heading also appears in the table of contents;
  scanning from there finds 5.1's Argon2d vector (same inputs). Anchor
  on "\n5.3.  Argon2id Test Vectors\n".
